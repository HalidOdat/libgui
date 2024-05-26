#include "Widget/SizedBox.hpp"
#include <Core/Color.hpp>

namespace Gui {

SizedBox::Handle SizedBox::create(float width, float height) {
  auto result = std::make_shared<SizedBox>(width, height);
  result->mFixedSizeWidget = true;
  return result;
}
SizedBox::Handle SizedBox::create(Vec2 size) {
  auto result = std::make_shared<SizedBox>(size);
  result->mFixedSizeWidget = true;
  return result;
}

Vec2 SizedBox::layout(Constraints) {
  // mSize.x = std::max(constraints.minWidth, std::min(constraints.maxWidth, mSize.x));
  // mSize.y = std::max(constraints.minHeight, std::min(constraints.maxHeight, mSize.y));
  return mSize;
}

void SizedBox::draw(Renderer2D& renderer) {
  renderer.drawQuad(mPosition, mSize, mColor);
}

SizedBox::Handle SizedBox::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  auto id = deserializeId(node["id"], errors);
  auto color = deserializeColor(node["color"], errors);

  float width = 0.0f;
  if (node.IsMap() && node["width"] && node["width"].IsScalar()) {
    width = node["width"].as<float>();
  }

  float height = 0.0f;
  if (node.IsMap() && node["height"] && node["height"].IsScalar()) {
    height = node["height"].as<float>();
  }

  auto result = SizedBox::create();
  result->setId(id);
  result->setColor(color);
  result->mSize = Vec2{width, height};
  return result;
}

} // namespace Gui
