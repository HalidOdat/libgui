#include "Widget/SizedBox.hpp"
#include <Core/Color.hpp>

namespace Gui {

SizedBoxWidget::Handle SizedBoxWidget::create(float width, float height) {
  return std::make_shared<SizedBoxWidget>(width, height);
}
SizedBoxWidget::Handle SizedBoxWidget::create(Vec2 size) {
  return std::make_shared<SizedBoxWidget>(size);
}

Vec2 SizedBoxWidget::layout(const Constraints& constraints) {
  mSize.x = std::max(constraints.minWidth, std::min(constraints.maxWidth, mSize.x));
  mSize.y = std::max(constraints.minHeight, std::min(constraints.maxHeight, mSize.y));
  return mSize;
}

void SizedBoxWidget::draw(Renderer2D& renderer) {
  renderer.drawQuad(mPosition, mSize, mColor);
}

} // namespace Gui
