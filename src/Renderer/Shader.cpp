#include <vector>
#include <array>
#include <algorithm>

#include "Core/OpenGL.hpp"
#include "Renderer/Shader.hpp"
#include "Utils/String.hpp"

#include <cstring>
#include <string>

using namespace std::literals;

#include <LibGuiAssets/assets.hpp>

namespace Gui {

  std::unordered_map<String, String> Shader::sGlobalDefines;

  bool Shader::globalDefine(String name, String content) {
    bool replaced = false;
    if (sGlobalDefines.find(name) != sGlobalDefines.end()) {
      replaced = true;
    }
    sGlobalDefines.insert_or_assign(std::move(name), std::move(content));
    return replaced;
  }

  static u32 shaderTypeToOpenGLType(Shader::Type type) {
    switch (type) {
      case Shader::Type::Vertex:   return GL_VERTEX_SHADER;
      case Shader::Type::Fragment: return GL_FRAGMENT_SHADER;
      case Shader::Type::Geometry: return GL_GEOMETRY_SHADER;
      case Shader::Type::Compute:  return GL_COMPUTE_SHADER;
    }
    GUI_UNREACHABLE("unknown shader type!");
  }

  static const char* shaderTypeToString(Shader::Type type) {
    switch (type) {
      case Shader::Type::Vertex:   return "Vertex";
      case Shader::Type::Fragment: return "Fragment";
      case Shader::Type::Geometry: return "Geometry";
      case Shader::Type::Compute:  return "Compute";
    }
    GUI_UNREACHABLE("unknown shader type!");
  }

  static const char* shaderVersionToStringDirectiveAndVersionMacros(Shader::Version version) {
    switch (version) {
      case Shader::Version::Es300:   return "#version 300 es\n";
      case Shader::Version::Core450: return "#version 450 core\n";
      case Shader::Version::Core330: return "#version 330 core\n";
    }
    GUI_UNREACHABLE("unknown shader version!");
  }

  static std::optional<Shader::Type> stringToShaderType(const StringView& string) {
    if (Utils::stringIsEqualIgnoreCase(string, shaderTypeToString(Shader::Type::Vertex))) {
      return Shader::Type::Vertex;
    } else if (Utils::stringIsEqualIgnoreCase(string, shaderTypeToString(Shader::Type::Fragment))) {
      return Shader::Type::Fragment;
    } else if (Utils::stringIsEqualIgnoreCase(string, shaderTypeToString(Shader::Type::Geometry))) {
      return Shader::Type::Geometry;
    } else if (Utils::stringIsEqualIgnoreCase(string, shaderTypeToString(Shader::Type::Compute))) {
      return Shader::Type::Compute;
    }

    return std::nullopt;
  }

  static std::vector<std::pair<Shader::Type, std::string>> parse(
    const Asset::Handle asset,
    Shader::Version version,
    const std::unordered_map<String, String>& definitions,
    const std::unordered_map<String, String>& globalDefines
  ) {
    auto filename = asset->filepath();
    Logger::info("Shader: %s", filename.c_str());
    const StringView typeDelimiter = "@type ";

    std::vector<std::pair<Shader::Type, std::string>> result;

    std::optional<std::string> common = std::nullopt;

    String stringDefinitions;
    for (auto&[name, content] : globalDefines) {
      stringDefinitions += "#define ";
      stringDefinitions += name;
      stringDefinitions += ' ';
      stringDefinitions += content;
      stringDefinitions += '\n';
    }

    for (auto&[name, content] : definitions) {
      stringDefinitions += "#define ";
      stringDefinitions += name;
      stringDefinitions += ' ';
      stringDefinitions += content;
      stringDefinitions += '\n';
    }

    if (!asset->load()) {
      Logger::error("cannot open file '%s': %s", filename.c_str(), strerror(errno));
      return {};
    }

    std::string type;
    std::string content;

    std::string_view file((char*)asset->data());
    while (!file.empty()) {
      auto position = file.find_first_of("\n");
      auto line = file.substr(0, position);

      Logger::info("-- %.*s", line.size(), line.data());
      if (position == std::string_view::npos) {
        if (file.empty()) {
          break;
        }
        auto lineEnd = file.find_first_not_of("\n");
        file.remove_prefix(lineEnd < file.size() ? lineEnd : file.size());
        line = file;
        file = ""sv;
      } else {
        file = file.substr(position + 1);
      }

      if (line.size() > typeDelimiter.size() && strncmp(line.data(), typeDelimiter.data(), typeDelimiter.size()) == 0) {
        if (!common) {
          common = String(shaderVersionToStringDirectiveAndVersionMacros(version)) + '\n' + stringDefinitions + content + '\n';
        } else {
          auto shaderType = stringToShaderType(type);
          if (!shaderType) {
            return {};
          }

          if (std::any_of(result.begin(), result.end(), [shaderType](const auto& value){
            return value.first == shaderType;
          })) {
            Logger::error("Duplicate shader type '%s' in %s", shaderTypeToString(*shaderType), asset->filepath().c_str());
            return {};
          }

          result.emplace_back(std::make_pair(*shaderType, *common + content));
        }

        content.clear();

        type = String(line).substr(typeDelimiter.size());
        continue;
      }

      content += line;
      content += '\n';
    }

    // FIXME: should free asset somehow
    // fclose(file);

    auto shaderType = stringToShaderType(type);
    if (!shaderType) {
      return {};
    }

    if (std::any_of(result.begin(), result.end(), [shaderType](const auto& value){
      return value.first == shaderType;
    })) {
      Logger::error("Duplicate shader type '%s' in %s", shaderTypeToString(*shaderType), asset->filepath().c_str());
      return {};
    }

    result.emplace_back(std::make_pair(*shaderType, *common + content));

    if (!std::any_of(result.begin(), result.end(), [](const auto& value){
        return value.first == Shader::Type::Vertex;
    }) || !std::any_of(result.begin(), result.end(), [](const auto& value){
        return value.first == Shader::Type::Fragment;
    })) {
      Logger::error("Shader program '%s' must contain a vertex and fragment shaders", asset->filepath().c_str());
      return {};
    }

    Logger::info("Loaded");

    return result;
  }

  Shader::Builder& Shader::Builder::version(Shader::Version version) {
    mVersion = version;
    return *this;
  }
  Shader::Builder& Shader::Builder::define(String name, String content) {
    // TODO: check for valid name
    mDefinitions.insert_or_assign(name, std::move(content));
    return *this;
  }

  Shader::Handle Shader::Builder::build() {
    auto parts = parse(mAsset, mVersion, mDefinitions, sGlobalDefines);
    if (parts.empty()) {
      return nullptr;
    }

    std::array<u32, SHADER_TYPE_COUNT> shaders = {0, 0, 0, 0};
    for (auto&[type, content] : parts) {
      u32 id = Shader::compile(type, content.c_str());

      shaders[(usize)type] = id;

      if (id == 0) {
        for (auto shader : shaders) {
          if (shader != 0) {
            glDeleteShader(shader);
          }
        }
        return nullptr;
      }
    }

    // link shaders
    u32 shaderProgram = glCreateProgram();

    for (auto shader : shaders) {
      if (shader != 0) {
        glAttachShader(shaderProgram, shader);
      }
    }

    glLinkProgram(shaderProgram);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        Logger::error("Shader program linking failed:\n%s\n", infoLog);
    }

    for (auto shader : shaders) {
      if (shader != 0) {
        glDetachShader(shaderProgram, shader);
        glDeleteShader(shader);
      }
    }

    if (!success) {
      glDeleteProgram(shaderProgram);
      return nullptr;
    }

    GLuint cameraBlockIndex = glGetUniformBlockIndex(shaderProgram, "Camera");
    if (cameraBlockIndex != GL_INVALID_INDEX) {
      glUniformBlockBinding(shaderProgram, cameraBlockIndex, 0);
      Logger::trace("Shader: Camera block binding at 0");
    }
    GLuint lightsBlockIndex = glGetUniformBlockIndex(shaderProgram, "Lights");
    if (lightsBlockIndex != GL_INVALID_INDEX) {
      glUniformBlockBinding(shaderProgram, lightsBlockIndex, 1);
      Logger::trace("Shader: Lights block binding at 1");
    }
    GLuint materialsBlockIndex = glGetUniformBlockIndex(shaderProgram, "Materials");
    if (materialsBlockIndex != GL_INVALID_INDEX) {
      glUniformBlockBinding(shaderProgram, materialsBlockIndex, 2);
      Logger::trace("Shader: Materials block binding at 2");
    }

    return std::make_shared<Shader>(shaderProgram, mAsset);
  }

  u32 Shader::compile(Type type, const char* source) noexcept {
    u32 id = glCreateShader(shaderTypeToOpenGLType(type));
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        Logger::error("%s Shader Compilation:\n%s\n", shaderTypeToString(type), infoLog);
        return NULL_SHADER;
    }

    return id;
  }

  Shader::Builder Shader::load(const Asset::Handle asset) {
    Builder builder;
    builder.mAsset = asset;
    return builder;
  }

  Shader::~Shader() {
    glDeleteProgram(id);
  }

  bool Shader::reload() {
    // if (!mFilePath.has_value()) {
    //   return false;
    // }

    // TODO: fix shader reload
    GUI_TODO("");

    // auto data = Shader::load(*mFilePath).build();
    // if (data) {
    //   Logger::trace("Reloaded shader: %s", mFilePath.value().c_str());

    //   glDeleteProgram(id);
    //   return true;
    // }

    return false;
  }

  void Shader::bind() noexcept {
    glUseProgram(id);
  }

  void Shader::unbind() noexcept {
    glUseProgram(NULL_SHADER);
  }

  i32 Shader::getUniformLocation(StringView string) {
    if (mUniformLocations.find(string) != mUniformLocations.end()) {
      return mUniformLocations.at(string);
    }
    i32 location = glGetUniformLocation(id, string.data());
    mUniformLocations[string] = location;
    return location;
  }

  void Shader::setFloat(StringView name, const f32 value) {
    i32 uniform_id = getUniformLocation(name);
    glUniform1f(uniform_id, value);
  }

  void Shader::setVec2(StringView name, const Vec2& value) {
    i32 uniform_id = getUniformLocation(name);
    glUniform2f(uniform_id, value.x, value.y);
  }

  void Shader::setVec3(StringView name, const Vec3& value) {
    i32 uniform_id = getUniformLocation(name);
    glUniform3f(uniform_id, value.x, value.y, value.z);
  }

  void Shader::setVec4(StringView name, const Vec4& value) {
    i32 uniform_id = getUniformLocation(name);
    glUniform4f(uniform_id, value.x, value.y, value.z, value.w);
  }

  void Shader::setMat2(StringView name, const Mat2& value) {
    i32 uniform_id = getUniformLocation(name);
    glUniformMatrix2fv(uniform_id, 1, GL_FALSE, &value[0][0]);
  }

  void Shader::setMat3(StringView name, const Mat3& value) {
    i32 uniform_id = getUniformLocation(name);
    glUniformMatrix3fv(uniform_id, 1, GL_FALSE, &value[0][0]);
  }

  void Shader::setMat4(StringView name, const Mat4& value) {
    i32 uniform_id = getUniformLocation(name);
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, &value[0][0]);
  }

  void Shader::setInt(StringView name, const i32 value) {
    i32 uniform_id = getUniformLocation(name);
    glUniform1i(uniform_id, value);
  }

  void Shader::setUint(StringView name, const u32 value) {
    i32 uniform_id = getUniformLocation(name);
    glUniform1ui(uniform_id, value);
  }

  void Shader::setIntArray(StringView name, const i32* value, u32 count) {
    i32 uniform_id = getUniformLocation(name);
    glUniform1iv(uniform_id, count, value);
  }

  void Shader::setUintArray(StringView name, const u32* value, u32 count) {
    i32 uniform_id = getUniformLocation(name);
    glUniform1uiv(uniform_id, count, value);
  }

} // namespace Gui

