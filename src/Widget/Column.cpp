#include "Widget/Column.hpp"

#include <iostream>

namespace Gui {

Column::Handle Column::create(Vec2 size) {
  auto result = std::make_shared<Column>(size);
  result->setAlignment(Alignment::Center);
  return result;
}

Vec2 Column::layout(Constraints constraints) {
  size_t fixedWidgetCount = 0;
  auto fixedWidgetWidth  = 0.0f;
  auto fixedWidgetHeight = 0.0f;

  auto paddingWidth = mPadding.w + mPadding.y;
  auto paddingHeight = mPadding.x + mPadding.z;

  auto position = Vec2{mPosition.x + mPadding.x, mPosition.y + mPadding.w};
  auto totalWidth  = 0.0f;
  auto totalHeight = 0.0f;

  if (mAlignment == Alignment::Center) {
    constraints.minWidth = constraints.maxWidth;
    constraints.minHeight = constraints.maxHeight;

    auto childConstraints = Constraints(
      0.0, 0.0,
      (constraints.maxWidth  - paddingWidth) / mChildren.size(),
      (constraints.maxHeight - paddingHeight)
    );

    for (auto& child : mChildren) {
      if (!child->mFixedSizeWidget) {
        continue;
      }

      fixedWidgetCount++;

      auto childSize = child->layout(childConstraints);
      fixedWidgetWidth += childSize.x;
      // fixedWidgetHeight += childSize.y;
    }
  }

  auto flexibleWidgetCount = mChildren.size() - fixedWidgetCount;
  if (!flexibleWidgetCount) {
    flexibleWidgetCount = 1;
  }

  auto childConstraints = Constraints(
    0.0, 0.0,
    (constraints.maxWidth  - fixedWidgetWidth  - paddingWidth) / flexibleWidgetCount,
    (constraints.maxHeight - fixedWidgetHeight - paddingHeight)
  );
  for (auto& child : mChildren) {
    child->setPosition(position); // Parent tells the child what position to be at!
    auto childSize = child->layout(childConstraints);

    totalWidth = childSize.x;
    totalHeight += std::max(totalHeight, childSize.y);

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

  auto result = Column::create();
  result->setId(id);
  result->setAlignment(Alignment::Center);
  result->mChildren = children;
  result->setColor(color);
  result->setPadding(Vec4{padding});
  return result;
}

} // namespace Gui
