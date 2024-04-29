#include "Widget/Clickable.hpp"
#include <Core/Color.hpp>

namespace Gui {

Clickable::Handle Clickable::create(Widget::Handle child, Callback callback) {
  return std::make_shared<Clickable>(child, callback);
}

Vec2 Clickable::layout(const Constraints& constraints) {
  return mChild->layout(constraints);
}

void Clickable::draw(Renderer2D& renderer) {
  mChild->draw(renderer);
}

} // namespace Gui
