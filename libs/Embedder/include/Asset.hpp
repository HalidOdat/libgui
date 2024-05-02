#pragma once

#include <string>
#include <memory>

namespace Gui {

  class Asset final {
  public:
    using Handle = std::shared_ptr<Asset>;

  public:
    ~Asset();
    // DISALLOW_COPY_AND_ASSIGN(Asset);

    static Asset::Handle path(std::string path);
    static Asset::Handle embedded(std::string path, const unsigned char* embedded, size_t size);

    inline bool isEmbedded() { return mEmbedded != nullptr; }
    inline const unsigned char* data() { return mData; }
    inline size_t size() { return mSize; }
    inline const std::string& filepath() const { return mPath; }

    bool load();
    bool reload();

  private:
    struct PathAsset {
      std::string path;
    };

    struct EmbeddedAsset {
      std::string path;
      const unsigned char* data;
      size_t size;
    };

  public:
    Asset(PathAsset);
    Asset(EmbeddedAsset);

  private:
    std::string mPath;

    const unsigned char* mEmbedded = nullptr;
    size_t mSize;

    unsigned char* mData;
  };

} // namespace Gui
