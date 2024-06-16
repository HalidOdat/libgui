#include "Widget/CheckBox.hpp"
#include <Core/Color.hpp>
#include <cctype>
#include <cmath>

namespace Gui {

CheckBox::Handle CheckBox::create(OnChangeCallback callback) {
  auto target = std::make_shared<CheckBox>(callback);
  target->setOnChange(std::move(callback));
  target->addClickEventHandler([target](auto) {
    target->mValue = !target->mValue;
    target->mOnChange(target->mValue);
    return true;
  });
  return target;
}

Vec2 CheckBox::layout(Constraints constraints) {
  mFixedWidthSizeWidget = !std::isinf(mWidth);
  mFixedHeightSizeWidget = !std::isinf(mHeight);
  mSize.x = mWidth;
  mSize.y = mHeight;
  return mSize;
}

void CheckBox::draw(Renderer2D& renderer) {
  auto position = mPosition + Vec2{mMargin.x, mMargin.y};
  auto size     = mSize - Vec2{mMargin.x, mMargin.y} - Vec2{mMargin.z, mMargin.w};

  if (mBackground != mBorderColor) {
    renderer.drawQuad(position, size, mBorderColor);
  }
  auto borederOffset = size * 0.25f;
  renderer.drawQuad(position + borederOffset / 2.0f, size - borederOffset, mBackground);
  
  auto offset = size * 0.6f;
  if (mValue) {
    renderer.drawQuad(position + offset / 2.0f, size - offset, mColor);
  }
}

CheckBox::Handle CheckBox::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  auto id = deserializeId(node["id"], errors);
  auto color = Color::WHITE;
  if (node["color"]) {
    color = deserializeColor(node["color"], errors);
  }
  auto background = Color::BLACK;
  if (node["background"]) {
    background = deserializeColor(node["background"], errors);
  }

  auto borderColor = Color::DARK_GRAY;
  if (node["border-color"]) {
    borderColor = deserializeColor(node["border-color"], errors);
  }

  float margin = 0.0f;
  if (node.IsMap() && node["margin"] && node["margin"].IsScalar()) {
    margin = node["margin"].as<float>();
  }

  float width = 40.0f;
  if (node.IsMap() && node["width"] && node["width"].IsScalar()) {
    width = node["width"].as<float>();
  }

  float height = 40.0f;
  if (node.IsMap() && node["height"] && node["height"].IsScalar()) {
    height = node["height"].as<float>();
  }

  bool display = true;
  if (node.IsMap() && node["display"] && node["display"].IsScalar()) {
    display = node["display"].as<bool>();
  }

  auto result = CheckBox::create([](bool){});
  result->setId(id);
  result->setColor(color);
  result->setBackground(background);
  result->setBorderColor(borderColor);
  result->setMargin(Vec4{margin});
  result->setWidth(width);
  result->setHeight(height);
  result->setDisplay(display);
  return result;
}

} // namespace Gui
