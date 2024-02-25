#pragma once

#include "Widget/Widget.hpp"

namespace Gui {

class SizedBoxWidget : public Widget {
public:
  using Handle = std::shared_ptr<SizedBoxWidget>;

public:
  static SizedBoxWidget::Handle create(float width = 0.0, float height = 0.0);
  static SizedBoxWidget::Handle create(Vec2 size);

  void setColor(Vec4 color) { mColor = color; }

  Vec2 layout(const Constraints& constraints) override;
  void draw(Renderer2D& renderer) override;

public: // Do NOT use these function use the create functions!
  SizedBoxWidget(float width = 0.0, float height = 0.0)
    : Widget({width, height})
  {}
  SizedBoxWidget(Vec2 size)
    : Widget(size)
  {}

private:
  Vec4 mColor;
};

} // namespace Gui
