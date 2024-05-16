#pragma once

#include "Widget/Widget.hpp"

namespace Gui {

class SizedBox : public Widget {
public:
  using Handle = std::shared_ptr<SizedBox>;

public:
  static SizedBox::Handle create(float width = 0.0, float height = 0.0);
  static SizedBox::Handle create(Vec2 size);

  void setColor(Vec4 color) { mColor = color; }

  Vec2 layout(Constraints constraints) override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Handle self, Widget::Visitor& visitor) override { return visitor(self); }

public: // Do NOT use these function use the create functions!
  SizedBox(float width = 0.0, float height = 0.0)
    : Widget({width, height})
  {}
  SizedBox(Vec2 size)
    : Widget(size)
  {}

private:
  Vec4 mColor;
};

} // namespace Gui
