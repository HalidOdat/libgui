#pragma once

#include <Asset.hpp>

#include <unordered_map>

namespace Gui {

  class RootAsset final {
  public:
    // DISALLOW_COPY_AND_ASSIGN(RootAsset);
    RootAsset(std::unordered_map<std::string, Asset::Handle> assets)
      : mAssets{std::move(assets)}
    {}

    Asset::Handle get(const std::string& file);

  private:
    std::unordered_map<std::string, Asset::Handle> mAssets{};
  };

} // namespace Gui
