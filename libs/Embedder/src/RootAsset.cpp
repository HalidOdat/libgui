#include <RootAsset.hpp>

namespace Gui {

  Asset::Handle RootAsset::get(const std::string& file) {
    auto value = mAssets.find(file);
    if (value == mAssets.end()) {
      return nullptr;
    }

    return value->second;
  }

} // namespace Gui
