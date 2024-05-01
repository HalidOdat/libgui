#include "Widget/Clickable.hpp"
#include <Core/Color.hpp>

namespace Gui {

Clickable::Handle Clickable::create(Widget::Handle child, Callback callback) {
  auto result = std::make_shared<Clickable>(child, callback);
  child->parent = result.get();
  return result;
}

Vec2 Clickable::layout(Constraints constraints) {
  mChild->setPosition(mPosition);
  mChild->layout(constraints);
  mSize = mChild->mSize;
  return mSize;
}

void Clickable::draw(Renderer2D& renderer) {
  mChild->draw(renderer);
}

} // namespace Gui
