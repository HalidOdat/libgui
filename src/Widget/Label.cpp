#include "Widget/Label.hpp"
#include <Core/Color.hpp>
#include <cctype>
#include <numeric>
#include <utility>

namespace Gui {

static std::pair<size_t, size_t> count(const std::string& s) {
  auto maxColumn = 0;
  auto column = 0;
  auto lines = std::accumulate(
    s.cbegin(),
    s.cend(),
    0,
    [&](int prev, char c) {
      maxColumn = std::max(maxColumn, column);
      if (c == '\n') {
        column = 0;
      }
      column++;

      return c != '\n' ? prev : prev + 1;
    }
  ) + 1;

  return std::make_pair(lines, maxColumn);
}

void Label::setText(std::string text) {
  mText = std::move(text);

  auto[lines, maxColumn] = count(mText);
  mLines = lines;
  mMaxColumn = maxColumn;
}

Label::Handle Label::create(std::string text, float fontSize) {
  auto result = std::make_shared<Label>(std::move(text), fontSize);

  auto[lines, maxColumn] = count(result->mText);
  result->mLines = lines;
  result->mMaxColumn = maxColumn;
  
  result->mFixedHeightSizeWidget = true;
  result->mFixedWidthSizeWidget  = true;
  return result;
}

Vec2 Label::layout(Constraints constraints) {
  mSize.x = (mFontSize - mFontSize/7.0f) * mMaxColumn + mMargin.x + mMargin.z;
  mSize.y = (mFontSize)                  * mLines + mMargin.y + mMargin.w;
  return mSize;
}

void Label::draw(Renderer2D& renderer) {
  renderer.drawText(
    mText,
    mPosition + Vec2{0.0f, mFontSize/7.0f} + Vec2{mMargin.x, mMargin.z},
    mFontSize,
    mColor
  );
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

  float margin = 0;
  if (node.IsMap() && node["margin"] && node["margin"].IsScalar()) {
    margin = node["margin"].as<float>();
  }

  auto color = Color::WHITE;
  if (node["color"]) {
    color = deserializeColor(node["color"], errors);
  }

  bool display = true;
  if (node.IsMap() && node["display"] && node["display"].IsScalar()) {
    display = node["display"].as<bool>();
  }

  auto result = Label::create(text, fontSize);
  result->setId(id);
  result->setColor(color);
  result->setMargin(Vec4{margin});
  result->setDisplay(display);
  return result;
}

} // namespace Gui
