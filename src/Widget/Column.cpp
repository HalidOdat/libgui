#include "Widget/Column.hpp"

#include <iostream>

namespace Gui {

Column::Handle Column::create(Vec2 size) {
  auto result = std::make_shared<Column>(size);
  result->setAlignment(Alignment::Center);
  return result;
}

Vec2 Column::layout(Constraints constraints) {
  mFixedWidthSizeWidget  = !std::isinf(mWidth);
  mFixedHeightSizeWidget = !std::isinf(mHeight);
  constraints.maxWidth   = std::min(constraints.maxWidth, mWidth);
  constraints.maxHeight  = std::min(constraints.maxHeight, mHeight);

  size_t fixedWidgetWidthCount  = 0;
  size_t fixedWidgetHeightCount = 0;
  auto fixedWidgetWidth  = 0.0f;
  auto fixedWidgetHeight = 0.0f;

  auto paddingWidth = mPadding.w + mPadding.y;
  auto paddingHeight = mPadding.x + mPadding.z;

  auto position = Vec2{mPosition.x + mPadding.x, mPosition.y + mPadding.w};
  auto totalWidth  = 0.0f;
  auto totalHeight = 0.0f;

  auto childConstraints = Constraints(
    0.0, 0.0,
    (constraints.maxWidth  - paddingWidth) / mChildren.size(),
    (constraints.maxHeight - paddingHeight)
  );

  for (auto& child : mChildren) {
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

  auto flexibleWidgetWidthCount = mChildren.size() - fixedWidgetWidthCount;
  auto flexibleWidgetHeightCount = mChildren.size() - fixedWidgetHeightCount;
  if (mAlignment == Alignment::Center) {
    constraints.minHeight = constraints.maxHeight;
    constraints.minWidth = constraints.maxWidth;
  } else if (mAlignment == Alignment::Vertical) {
    constraints.minWidth = constraints.maxWidth;
  } else if (mAlignment == Alignment::Horizontal) {
    constraints.minHeight = constraints.maxHeight;
  }
  if (!flexibleWidgetWidthCount) {
    flexibleWidgetWidthCount = 1;

    auto spaceLeft = constraints.maxWidth - fixedWidgetWidth;
    if (spaceLeft >= 0.0f) {
      switch (mMainAxis) {
        case MainAxis::Start:
          position.x += 0.0f;
          break;
        case MainAxis::End:
          position.x += spaceLeft;
          break;
        case MainAxis::Center:
          position.x += spaceLeft / 2.0f;
          break;
      }
    }
  }

  if (!flexibleWidgetHeightCount) {
    flexibleWidgetHeightCount = 1;
      
    auto spaceLeft = constraints.maxHeight - fixedWidgetHeight;
    if (spaceLeft >= 0.0f) {
      switch (mCrossAxis) {
        case CrossAxis::Start:
          position.y += 0.0f;
          break;
        case CrossAxis::End:
          position.y += spaceLeft;
          break;
        case CrossAxis::Center:
          position.y += spaceLeft / 2.0f;
          break;
      }
    }
  }

  childConstraints = Constraints(
    0.0, 0.0,
    (constraints.maxWidth  - paddingWidth) / flexibleWidgetWidthCount,
    (constraints.maxHeight - paddingHeight)
  );
  for (auto& child : mChildren) {
    child->setPosition(position); // Parent tells the child what position to be at!
    auto childSize = child->layout(childConstraints);

    totalWidth += childSize.x;
    totalHeight = std::max(totalHeight, childSize.y);

    position.x  += childSize.x;
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

Column::Handle Column::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
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

  auto mainAxis = MainAxis::Center;
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

  auto crossAxis = CrossAxis::Center;
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

  auto result = Column::create();
  result->setId(id);
  result->setAlignment(Alignment::Center);
  result->mChildren = children;
  result->setColor(color);
  result->setPadding(Vec4{padding});
  result->setWidth(width);
  result->setHeight(height);
  result->setMainAxis(mainAxis);
  result->setCrossAxis(crossAxis);
  return result;
}

} // namespace Gui
