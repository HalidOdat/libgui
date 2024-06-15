#pragma once

#include <functional>
#include <Events/MouseEvent.hpp>
#include "Widget/Widget.hpp"

namespace Gui {

class Label : public Widget {
public:
  using Handle = std::shared_ptr<Label>;

public:
  static Label::Handle create(std::string text = "", float fontSize = 28);

  Vec2 layout(Constraints constraints) override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Handle self, Widget::Visitor& visitor) override { return visitor(self); }

  void setFontSize(float size) { mFontSize = size; }
  void setColor(Vec4 value) { mColor = value; }
  Vec4 getColor() const { return mColor; }

  static Label::Handle deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors);

public: // Do NOT use these function use the create functions!
  Label(std::string text, float fontSize)
    : mText{std::move(text)}, mFontSize{fontSize}
  {}

private:
  std::string mText;
  float mFontSize;
  Vec4 mColor = Color::BLACK;
};

} // namespace Gui
