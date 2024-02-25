#pragma once

#include "Core/Base.hpp"
#include "Widget/Widget.hpp"

namespace Gui {

class ContainerWidget : public Widget {
public:
  using Handle = std::shared_ptr<ContainerWidget>;

public:
  static ContainerWidget::Handle create(float width = 0.0, float height = 0.0);
  static ContainerWidget::Handle create(Vec2 size);

  void addChild(Widget::Handle child);
  void setColor(Vec4 color) { mColor = color; }

  Vec2 layout(const Constraints& constraints) override;
  void reportSize() const override;
  void draw(Renderer2D& renderer) override;

public: // Do NOT use these function use the create functions!
  ContainerWidget(float width = 0.0, float height = 0.0)
    : Widget({width, height})
  {}
  ContainerWidget(Vec2 size)
    : Widget(size)
  {}

private:
  std::vector<Widget::Handle> mChildren;
  Vec4 mColor;
};

} // namespace Gui
