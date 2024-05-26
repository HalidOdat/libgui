#pragma once

#include <Events/MouseEvent.hpp>
#include "Widget/Widget.hpp"

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

  inline void setFontSize(float size) { mFontSize = size; }
  inline float getFontSize() const { return mFontSize; }
  inline void setColor(Vec4 color) { mColor = color; }
  inline Vec4 getColor() const { return mColor; }
  inline void setBackground(Vec4 color) { mBackground = color; }
  inline Vec4 getBackground() const { return mBackground; }

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
};

} // namespace Gui
