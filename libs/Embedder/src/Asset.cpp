#include <Asset.hpp>

#include <cstdio>
#include <cstring>
#include <cerrno>

namespace Gui {

  std::pair<unsigned char*, size_t> fileToString(std::string_view filename) {
    FILE* f = fopen(filename.data(), "rb");
    if (!f) {
      fprintf(stderr, "[ERROR]: Couldn't open file '%.*s': %s", (int)filename.size(), filename.data(), strerror(errno));
      return {nullptr, 0};
    }

    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    fseek(f, 0, SEEK_SET);
    size_t size = sizeof(unsigned char) * (length + 1);
    unsigned char* buffer = (unsigned char*)malloc(size);
    if (!buffer) {
      fprintf(stderr, "[ERROR]: Couldn't allocate %zu for file '%s'", size, filename.data());
      fclose(f);
      return {nullptr, 0};
    }
    fread(buffer, sizeof(unsigned char), length, f);
    fclose(f);
    buffer[length] = '\0';
    return {buffer, length};
  }

  Asset::Asset(PathAsset value)
    : mPath{std::move(value.path)}
  {
    auto[data, length] = fileToString(mPath.c_str());
    mData = data;
    mSize = length;
  }
  Asset::Asset(EmbeddedAsset value)
    : mPath{std::move(value.path)},
      mEmbedded{value.data},
      mSize{value.size},
      mData{(unsigned char*)value.data}
  {}
  Asset::~Asset() {
    if (!isEmbedded()) {
      free(mData);
    }
  }

  Asset::Handle Asset::path(std::string path) {
    return std::make_shared<Asset>(PathAsset { path });
  }

  Asset::Handle Asset::embedded(std::string path, const unsigned char* embedded, size_t size) {
    return std::make_shared<Asset>(EmbeddedAsset { path, embedded, size });
  }

  bool Asset::load() {
    if (isEmbedded()) {
      return true;
    }

    free((char*)mData);

    auto[data, length] = fileToString(mPath.c_str());
    mData = data;
    mSize = length;

    return mData != nullptr;
  }

  bool Asset::reload() {
    return load();
  }

} // namespace Gui
