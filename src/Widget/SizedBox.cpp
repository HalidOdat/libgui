#include "Widget/SizedBox.hpp"
#include <Core/Color.hpp>

namespace Gui {

SizedBox::Handle SizedBox::create(float width, float height) {
  return std::make_shared<SizedBox>(width, height);
}
SizedBox::Handle SizedBox::create(Vec2 size) {
  return std::make_shared<SizedBox>(size);
}

Vec2 SizedBox::layout(Constraints constraints) {
  mSize.x = std::max(constraints.minWidth, std::min(constraints.maxWidth, mSize.x));
  mSize.y = std::max(constraints.minHeight, std::min(constraints.maxHeight, mSize.y));
  return mSize;
}

void SizedBox::draw(Renderer2D& renderer) {
  renderer.drawQuad(mPosition, mSize, mColor);
}

} // namespace Gui
