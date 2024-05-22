#include <RootAsset.hpp>
#include <algorithm>

namespace Gui {

  Asset::Handle RootAsset::get(const std::string& file) {
    auto path = file;
    std::replace(path.begin(), path.end(), '\\', '/');

    auto value = mAssets.find(path);
    if (value == mAssets.end()) {
      return nullptr;
    }

    return value->second;
  }

} // namespace Gui
