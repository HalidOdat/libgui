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
  mSize.y = std::max(constraints.minHeight, std::min(constraints.maxHeight, mFontSize * 2.0f));
  return mSize;
}

void Button::draw(Renderer2D& renderer) {
  renderer.drawQuad(
    mPosition,
    mSize,
    mBackground
  );
  auto charWidth = (mFontSize - mFontSize/7.0f);
  auto sizeUnits = (mSize.x / charWidth - mText.size()) / 2.0f * charWidth;
  auto offset = Vec2{
    sizeUnits,
    mFontSize/2.0f,
  };
  renderer.drawText(mText, mPosition + offset, mFontSize, mColor);
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

  auto result = Button::create(text, fontSize);
  result->setId(id);
  result->setColor(color);
  result->setBackground(background);
  return result;
}

} // namespace Gui
