#include "Widget/Row.hpp"

#include <iostream>

namespace Gui {

Row::Handle Row::create(Vec2 size) {
  return std::make_shared<Row>(size);
}

Row::Handle Row::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
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

  bool display = true;
  if (node.IsMap() && node["display"] && node["display"].IsScalar()) {
    display = node["display"].as<bool>();
  }

  auto result = Row::create();
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
