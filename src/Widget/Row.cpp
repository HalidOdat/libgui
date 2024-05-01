#include "Widget/Row.hpp"

#include <iostream>

namespace Gui {

Row::Handle Row::create(Vec2 size) {
  return std::make_shared<Row>(size);
}

} // namespace Gui
