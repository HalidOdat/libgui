#include "Widget/Button.hpp"
#include <Core/Color.hpp>
#include <cctype>

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
  mSize.x = constraints.maxWidth;
  mSize.y = constraints.maxHeight;
  return mSize;
}

void Button::draw(Renderer2D& renderer) {
  renderer.drawQuad(
    mPosition + Vec2{mMargin.x, mMargin.y},
    mSize - Vec2{mMargin.x, mMargin.y} - Vec2{mMargin.z, mMargin.w},
    mBackground
  );
  auto charWidth = (mFontSize - mFontSize/7.0f);
  auto sizeUnitsWidth = ((mSize.x - mMargin.x - mMargin.z) / charWidth - mText.size()) / 2.0f * charWidth;

  auto charHeight = mFontSize;
  auto sizeUnitsHeigth = ((mSize.y - mMargin.y - mMargin.w) / charHeight) / 2.0f * charHeight;
  auto offset = Vec2{
    sizeUnitsWidth,
    sizeUnitsHeigth,
  };
  renderer.drawText(mText, mPosition + Vec2{mMargin.x, mMargin.y} + offset, mFontSize, mColor);
}

Button::Handle Button::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  auto id = deserializeId(node["id"], errors);
  auto color = deserializeColor(node["color"], errors);
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

  auto result = Button::create(text, fontSize);
  result->setId(id);
  result->setColor(color);
  result->setBackground(background);
  result->setMargin(Vec4{margin});
  return result;
}

} // namespace Gui
