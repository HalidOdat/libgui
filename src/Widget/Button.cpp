#include "Widget/Button.hpp"
#include <Core/Color.hpp>
#include <cctype>
#include <cmath>

namespace Gui {

Button::Handle Button::create(Widget::ClickCallback callback, std::string text, float fontSize) {
  auto target = std::make_shared<Button>(std::move(text), fontSize);
  target->addClickEventHandler(std::move(callback));
  return target;
}

Button::Handle Button::create(std::string text, float fontSize) {
  auto target = std::make_shared<Button>(std::move(text), fontSize);
  return target;
}

Vec2 Button::layout(Constraints constraints) {
  mFixedWidthSizeWidget = !std::isinf(mWidth);
  mFixedHeightSizeWidget = !std::isinf(mHeight);
  mSize.x = std::min(mWidth, constraints.maxWidth);
  mSize.y = std::min(mHeight, constraints.maxHeight);
  return mSize;
}

void Button::draw(Renderer2D& renderer) {
  auto position = mPosition + Vec2{mMargin.x, mMargin.y};
  auto size     = mSize - Vec2{mMargin.x, mMargin.y} - Vec2{mMargin.z, mMargin.w};

  renderer.drawQuad(position, size, mBackground);

  auto charWidth = (mFontSize - mFontSize/7.0f);
  auto sizeUnitsWidth = (size.x / charWidth - mText.size()) / 2.0f * charWidth - charWidth / 2.0f;

  auto charHeight = mFontSize;
  auto sizeUnitsHeigth = (size.y / charHeight) / 2.0f * charHeight - charHeight / 2.0f;
  auto offset = Vec2{
    sizeUnitsWidth,
    sizeUnitsHeigth,
  };
  renderer.drawText(mText, position + offset, mFontSize, mColor);
}

Button::Handle Button::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  auto id = deserializeId(node["id"], errors);
  auto color = Color::WHITE;
  if (node["color"]) {
    color = deserializeColor(node["color"], errors);
  }
  auto background = deserializeColor(node["background"], errors);

  std::string text;
  if (node.IsMap() && node["text"] && node["text"].IsScalar()) {
    text = node["text"].as<std::string>();
  }

  float fontSize = 28;
  if (node.IsMap() && node["font-size"] && node["font-size"].IsScalar()) {
    fontSize = node["font-size"].as<float>();
  }

  float margin = 28;
  if (node.IsMap() && node["margin"] && node["margin"].IsScalar()) {
    margin = node["margin"].as<float>();
  }

  float width = INFINITY;
  if (node.IsMap() && node["width"] && node["width"].IsScalar()) {
    width = node["width"].as<float>();
  }

  float height = INFINITY;
  if (node.IsMap() && node["height"] && node["height"].IsScalar()) {
    height = node["height"].as<float>();
  }

  auto result = Button::create(text, fontSize);
  result->setId(id);
  result->setColor(color);
  result->setBackground(background);
  result->setMargin(Vec4{margin});
  result->setWidth(width);
  result->setHeight(height);
  return result;
}

} // namespace Gui
