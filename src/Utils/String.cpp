#include "Utils/String.hpp"

#include <algorithm>

namespace Gui::Utils {

  bool stringIsEqualIgnoreCase(const StringView& rhs, const StringView& lhs) {
    return std::equal(
      rhs.begin(), rhs.end(),
      lhs.begin(), lhs.end(),
      [](char a, char b) {
        return tolower(a) == tolower(b);
      }
    );
  }

} // namespace Gui::Utils

