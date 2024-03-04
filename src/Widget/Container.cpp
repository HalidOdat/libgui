#include "Widget/Container.hpp"

#include <iostream>

namespace Gui {

Container::Handle Container::create(Vec2 size) {
  return std::make_shared<Container>(size);
}

void Container::addChild(Widget::Handle child) {
  child->parent = this;
  mChildren.push_back(std::move(child));
}

Vec2 Container::layout(Constraints constraints) {
  if (mAlignment == Alignment::Center) {
    constraints.minWidth = constraints.maxWidth;
    constraints.minHeight = constraints.maxHeight;
  }

  auto paddingWidth = mPadding.w + mPadding.y;
  auto paddingHeight = mPadding.x + mPadding.z;

  auto position = Vec2{mPosition.x + mPadding.x, mPosition.y + mPadding.w};
  auto totalWidth  = 0.0f;
  auto totalHeight = 0.0f;

  auto childConstraints = Constraints(
    0.0, 0.0,
    (constraints.maxWidth - paddingWidth),
    (constraints.maxHeight - paddingHeight) / mChildren.size()
  );
  for (auto& child : mChildren) {
    child->setPosition(position); // Parent tells the child what position to be at!
    auto childSize = child->layout(childConstraints);

    totalWidth = std::max(totalWidth, childSize.x);
    totalHeight += childSize.y;

    position.y  += childSize.y;
  }

  // x --> top
  // y --> right
  // z --> bottom
  // w --> left
  //
  // TODO: Maybe having just VecN (N = 2,3,4) does not convey the meaning,
  //       Add structure for padding with the named fields.
  totalWidth  += paddingWidth;
  totalHeight += paddingHeight;

  mSize.x = std::max(constraints.minWidth, std::min(constraints.maxWidth, totalWidth));
  mSize.y = std::max(constraints.minHeight, std::min(constraints.maxHeight, totalHeight));
  return mSize;
}

void Container::reportSize() const {
  std::cout << "Container Size: x=" << mPosition.x << ", y=" << mPosition.y << ", width=" << mSize.x << ", height=" << mSize.y << std::endl;
  for (auto& child : mChildren) {
    child->reportSize();
  }
}

void Container::draw(Renderer2D& renderer) {
  renderer.drawQuad(mPosition, mSize, mColor);
  for (auto child : mChildren) {
    child->draw(renderer);
  }
}

} // namespace Gui