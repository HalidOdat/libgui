#pragma once

#include <Events/MouseEvent.hpp>
#include "Widget/Widget.hpp"
#include <cmath>

namespace Gui {

class Button : public Widget {
public:
  using Handle = std::shared_ptr<Button>;

public:
  static Button::Handle create(Widget::ClickCallback callback, std::string text = "", float fontSize = 28);
  static Button::Handle create(std::string text = "", float fontSize = 28);

  Vec2 layout(Constraints constraints) override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Handle self, Widget::Visitor& visitor) override { return visitor(self); }

  inline void setText(std::string text) { mText = text; }
  inline std::string getText() const { return mText; }
  inline void setFontSize(float size) { mFontSize = size; }
  inline float getFontSize() const { return mFontSize; }
  inline void setColor(Vec4 color) { mColor = color; }
  inline Vec4 getColor() const { return mColor; }
  inline void setBackground(Vec4 color) { mBackground = color; }
  inline Vec4 getBackground() const { return mBackground; }
  inline void setMargin(Vec4 color) { mMargin = color; }
  inline Vec4 getMargin() const { return mMargin; }
  inline void setWidth(float size) { mWidth = size; }
  inline float getWidth() const { return mWidth; }
  inline void setHeight(float size) { mHeight = size; }
  inline float getHeight() const { return mHeight; }

  static Button::Handle deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors);

public: // Do NOT use these function use the create functions!
  Button(std::string text, float fontSize)
    : mText{std::move(text)}, mFontSize{fontSize}
  {}

private:
  std::string mText;
  float mFontSize;
  Vec4 mColor;
  Vec4 mBackground;
  Vec4 mMargin{};

  float mWidth{INFINITY};
  float mHeight{INFINITY};
};

} // namespace Gui
