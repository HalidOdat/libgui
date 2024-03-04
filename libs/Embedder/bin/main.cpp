#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstdint>

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

using namespace std;
using namespace std::filesystem;

// TODO: Look into _CRT_SECURE_NO_WARNINGS it warns on windows cl
//       builds in the CI.

constexpr size_t ROW_COUNT = 30;

void print_usage() {
  printf(
    "ResourceEmbedder <input> <output>\n"
  );
}

int write_or_die(FILE* f, const char* format, ...) {
  va_list ap;
  va_start(ap, format);

  int ret = vfprintf(f, format, ap);
  if (ret < 0) {
    perror("couldn't write to file");
    exit(10);
  }

  va_end(ap);

  return ret;
}

std::string get_name(size_t index, std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
        [](char c){ return isalpha(c) ? c : '_'; });
  return "asset_" + std::to_string(index) + "__" + str;
}

struct Asset {
  std::string path;
  std::string name;
  size_t count;
};

int main(int argc, char* argv[]) {
  if (argc != 3) {
    print_usage();
    return 1;
  }

  auto source = std::string(argv[2]) + ".cpp";
  FILE* output_file = fopen(source.c_str(), "w+");
  if (!output_file) {
    perror("Couldn't create output file");
    return 2;
  }

  write_or_die(output_file, "#include <RootAsset.hpp>\n");

  auto root = std::filesystem::canonical(argv[1]);

  std::vector<Asset> assets;

  // TODO: Handle single files
  size_t count = 0;
  uint8_t buffer[ROW_COUNT];
  for (std::filesystem::recursive_directory_iterator i(root), end; i != end; ++i) {
    if (is_directory(i->path())) {
      continue;
    }

    auto path          = std::filesystem::canonical(i->path());
    auto relative_path = std::filesystem::relative(path, root.parent_path());
    auto name          = get_name(count, relative_path.string());

    // cout << path << " ---> " << name << "\n";

    write_or_die(output_file,
      "\n"
      "const static unsigned char "
    );
    write_or_die(output_file, name.c_str());
    write_or_die(output_file, "[] = {");

    std::string temp = path.string();
    FILE* input_file = fopen(temp.c_str(), "rb");
    if (!input_file) {
      perror("Couldn't open input file");
      return 2;
    }

    size_t sum = 0;
    size_t ret;
    while (!feof(input_file) && !ferror(input_file)) {
      ret = fread(buffer, sizeof(uint8_t), ROW_COUNT, input_file);

      write_or_die(output_file, "\n");

      for (size_t i = 0; i < ret; ++i) {
        write_or_die(output_file, "0x%02x,", (int)buffer[i]);
      }

      sum += ret;
    }

    if (ferror(input_file)) {
      perror("error while reading the input file");
      return 3;
    }

    fclose(input_file);

    write_or_die(output_file,
      "\n};\n"
    );
  
    assets.push_back(Asset{
      relative_path.string(),
      name,
      sum
    });

    count++;
  }

  write_or_die(output_file, "\nGui::RootAsset ");
  std::string temp = root.filename().string();
  write_or_die(output_file, temp.c_str());
  write_or_die(output_file, "({\n");
  for (auto&[path, name, sum] : assets) {
    write_or_die(output_file, "  {R\"#10#(");
    write_or_die(output_file, path.c_str());
    write_or_die(output_file, ")#10#\", Gui::Asset::embedded(");
    write_or_die(output_file, name.c_str());
    write_or_die(output_file, ", ");
    write_or_die(output_file, std::to_string(sum).c_str());
    write_or_die(output_file, ")");
    write_or_die(output_file, "},\n");
  }
  write_or_die(output_file, "});\n");

  fclose(output_file);

  auto header = std::string(argv[2]) + ".hpp";
  output_file = fopen(header.c_str(), "w+");
  if (!output_file) {
    perror("Couldn't create output file");
    return 2;
  }

  write_or_die(output_file,
    "#pragma once\n"
    "\n"
    "#include <RootAsset.hpp>\n"
    "\n"
    "extern Gui::RootAsset "
  );
  write_or_die(output_file, temp.c_str());
  write_or_die(output_file, ";\n");

  fclose(output_file);

  return 0;
}
