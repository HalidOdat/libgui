#include "Widget/Label.hpp"
#include <Core/Color.hpp>
#include <cctype>

namespace Gui {

Label::Handle Label::create(std::string text, float fontSize) {
  auto result = std::make_shared<Label>(std::move(text), fontSize);
  result->mFixedHeightSizeWidget = true;
  return result;
}

Vec2 Label::layout(Constraints constraints) {
  mSize.x = constraints.maxWidth;
  mSize.y = std::max(constraints.minHeight, std::min(constraints.maxHeight, mFontSize * 2.0f));
  return mSize;
}

void Label::draw(Renderer2D& renderer) {
  renderer.drawText(mText, mPosition + mFontSize/2.0f, mFontSize, mColor);
}

Label::Handle Label::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  auto id = deserializeId(node["id"], errors);

  std::string text;
  if (node.IsMap() && node["text"] && node["text"].IsScalar()) {
    text = node["text"].as<std::string>();
  }

  float fontSize = 28;
  if (node.IsMap() && node["font-size"] && node["font-size"].IsScalar()) {
    fontSize = node["font-size"].as<float>();
  }

  auto color = deserializeColor(node["color"], errors);

  auto result = Label::create(text, fontSize);
  result->setId(id);
  result->setColor(color);
  return result;
}

} // namespace Gui
