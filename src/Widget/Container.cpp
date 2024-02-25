#include "Widget/Container.hpp"

#include <iostream>

namespace Gui {

ContainerWidget::Handle ContainerWidget::create(float width, float height) {
  return std::make_shared<ContainerWidget>(width, height);
}
ContainerWidget::Handle ContainerWidget::create(Vec2 size) {
  return std::make_shared<ContainerWidget>(size);
}

void ContainerWidget::addChild(Widget::Handle child) {
  mChildren.push_back(std::move(child));
}

Vec2 ContainerWidget::layout(const Constraints& constraints) {
  float totalWidth = 0;
  float totalHeight = 0;
  for (auto& child : mChildren) {
      auto childSize = child->layout(constraints);
      totalWidth = std::max(totalWidth, childSize.x);
      totalHeight += childSize.y;
  }

  mSize.x = std::max(constraints.minWidth, std::min(constraints.maxWidth, totalWidth));
  mSize.y = std::max(constraints.minHeight, std::min(constraints.maxHeight, totalHeight));
  return mSize;
}

void ContainerWidget::reportSize() const {
  std::cout << "Container Size: x=" << mPosition.x << ", y=" << mPosition.y << ", width=" << mSize.x << ", height=" << mSize.y << std::endl;
  for (auto& child : mChildren) {
    child->reportSize();
  }
}

void ContainerWidget::draw(Renderer2D& renderer) {
  renderer.drawQuad(mPosition, mSize, mColor);
  for (auto child : mChildren) {
    child->draw(renderer);
  }
}

} // namespace Gui
