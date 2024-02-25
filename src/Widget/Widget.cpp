#include "Widget/Widget.hpp"

#include <iostream>

namespace Gui {

void Widget::reportSize() const {
  std::cout
    << "Size: x="
    << mPosition.x
    << ", y="
    << mPosition.y
    << ", width="
    << mSize.x
    << ", height="
    << mSize.y
    << std::endl;
}

} // namespace Gui
