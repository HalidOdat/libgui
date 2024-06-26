#include "Widget/Container.hpp"

#include <iostream>
#include <cmath>

namespace Gui {

Container::Handle Container::create(Vec2 size) {
  return std::make_shared<Container>(size);
}

void Container::addChild(Widget::Handle child) {
  child->parent = this;
  mChildren.push_back(std::move(child));
}

Vec2 Container::layout(Constraints constraints) {
  mFixedWidthSizeWidget  = !std::isinf(mWidth);
  mFixedHeightSizeWidget = !std::isinf(mHeight);
  constraints.maxWidth   = std::min(constraints.maxWidth, mWidth);
  constraints.maxHeight  = std::min(constraints.maxHeight, mHeight);

  if (mAlignment == Alignment::Center) {
    constraints.minWidth = constraints.maxWidth;
    constraints.minHeight = constraints.maxHeight;
  } else if (mAlignment == Alignment::Vertical) {
    constraints.minWidth = constraints.maxWidth;
  } else if (mAlignment == Alignment::Horizontal) {
    constraints.minHeight = constraints.maxHeight;
  }

  if (mFixedWidthSizeWidget) {
    constraints.minWidth = mWidth;
  }
  if (mFixedHeightSizeWidget) {
    constraints.minHeight = mHeight;
  }

  size_t fixedWidgetWidthCount = 0;
  size_t fixedWidgetHeightCount = 0;
  auto fixedWidgetWidth  = 0.0f;
  auto fixedWidgetHeight = 0.0f;

  auto paddingWidth = mPadding.w + mPadding.y;
  auto paddingHeight = mPadding.x + mPadding.z;

  auto position = Vec2{mPosition.x + mPadding.x, mPosition.y + mPadding.w};
  auto totalWidth  = 0.0f;
  auto totalHeight = 0.0f;

  auto actualChildrenCount = 0;

  auto childConstraints = Constraints(
    0.0, 0.0,
    (constraints.maxWidth  - paddingWidth),
    (constraints.maxHeight - paddingHeight) / mChildren.size()
  );

  for (auto& child : mChildren) {
    if (!child->mDisplay) {
      continue;
    }
    actualChildrenCount++;
    auto childSize = child->layout(childConstraints);
    if (child->mFixedWidthSizeWidget) {
      fixedWidgetWidthCount++;
      fixedWidgetWidth += childSize.x;
    }
    if (child->mFixedHeightSizeWidget) {
      fixedWidgetHeightCount++;
      fixedWidgetHeight += childSize.y;
    }
  }

  auto flexibleWidgetWidthCount = actualChildrenCount - fixedWidgetWidthCount;
  auto flexibleWidgetHeightCount = actualChildrenCount - fixedWidgetHeightCount;
  if (!flexibleWidgetWidthCount) {
    flexibleWidgetWidthCount = 1;
      
    auto spaceLeft = constraints.maxWidth - fixedWidgetWidth;
    if (spaceLeft > 0.0f) {
      switch (mCrossAxis) {
        case CrossAxis::Start:
          position.x += 0.0f;
          break;
        case CrossAxis::End:
          position.x += spaceLeft;
          break;
        case CrossAxis::Center:
          position.x += spaceLeft / 2.0f;
          break;
      }
    }
  }
  if (!flexibleWidgetHeightCount) {
    flexibleWidgetHeightCount = 1;
      
    auto spaceLeft = constraints.maxHeight - fixedWidgetHeight;
    if (spaceLeft > 0.0f) {
      switch (mMainAxis) {
        case MainAxis::Start:
          position.y += 0.0f;
          break;
        case MainAxis::End:
          position.y += spaceLeft;
          break;
        case MainAxis::Center:
          position.y += spaceLeft / 2.0f;
          break;
      }
    }
  }

  childConstraints = Constraints(
    0.0, 0.0,
    (constraints.maxWidth - paddingWidth),
    (constraints.maxHeight - fixedWidgetHeight - paddingHeight) / flexibleWidgetHeightCount
  );

  for (auto& child : mChildren) {
    if (!child->mDisplay) {
      continue;
    }

    child->setPosition(position); // Parent tells the child what position to be at!
    auto childSize = child->layout(childConstraints);

    totalWidth   = std::max(totalWidth, childSize.x);
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
    if (!child->mDisplay) {
      continue;
    }
    child->draw(renderer);
  }
}

Container::Handle Container::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  if (!node.IsMap()) {
    insertDeserializationError(errors, node.Mark(), "Expected widget node to be a YAML map");
    return nullptr;
  }

  auto id = deserializeId(node["id"], errors);
  auto children = deserializeChildren(node["children"], errors);
  auto color = deserializeColor(node["color"], errors);
  auto padding = deserializePadding(node["padding"], errors);

  float width = INFINITY;
  if (node.IsMap() && node["width"] && node["width"].IsScalar()) {
    width = node["width"].as<float>();
  }

  float height = INFINITY;
  if (node.IsMap() && node["height"] && node["height"].IsScalar()) {
    height = node["height"].as<float>();
  }

  auto mainAxis = MainAxis::Start;
  if (node.IsMap() && node["main-axis"] && node["main-axis"].IsScalar()) {
    auto value = node["main-axis"].as<std::string>();
    if (value == "start") {
      mainAxis = MainAxis::Start;
    } else if (value == "end"){
      mainAxis = MainAxis::End;
    } else if (value == "center") {
      mainAxis = MainAxis::Center;
    } else {
      insertDeserializationError(errors, node["main-axis"].Mark(), "unknown main-axis type: " + value);
    }
  }

  auto crossAxis = CrossAxis::Start;
  if (node.IsMap() && node["cross-axis"] && node["cross-axis"].IsScalar()) {
    auto value = node["cross-axis"].as<std::string>();
    if (value == "start") {
      crossAxis = CrossAxis::Start;
    } else if (value == "end"){
      crossAxis = CrossAxis::End;
    } else if (value == "center") {
      crossAxis = CrossAxis::Center;
    } else {
      insertDeserializationError(errors, node["cross-axis"].Mark(), "unknown main-axis type: " + value);
    }
  }

  bool display = true;
  if (node.IsMap() && node["display"] && node["display"].IsScalar()) {
    display = node["display"].as<bool>();
  }

  auto result = Container::create();
  result->setId(id);
  result->setAlignment(Alignment::Center);
  result->mChildren = children;
  result->setColor(color);
  result->setPadding(Vec4{padding});
  result->setWidth(width);
  result->setHeight(height);
  result->setMainAxis(mainAxis);
  result->setCrossAxis(crossAxis);
  result->setDisplay(display);
  return result;
}

} // namespace Gui
