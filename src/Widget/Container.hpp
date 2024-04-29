#pragma once

#include "Core/Base.hpp"
#include "Widget/Widget.hpp"

namespace Gui {

class Container : public Widget {
public:
  using Handle = std::shared_ptr<Container>;

public:
  static Container::Handle create(float width = 0.0, float height = 0.0);
  static Container::Handle create(Vec2 size);

  void addChild(Widget::Handle child);
  void setColor(Vec4 color) { mColor = color; }
  void setPadding(Vec4 padding) { mPadding = padding; }

  Vec2 layout(const Constraints& constraints) override;
  void reportSize() const override;
  void draw(Renderer2D& renderer) override;

public: // Do NOT use these function use the create functions!
  Container(float width = 0.0, float height = 0.0)
    : Widget({width, height})
  {}
  Container(Vec2 size)
    : Widget(size)
  {}

private:
  std::vector<Widget::Handle> mChildren;
  Vec4 mColor;

  Vec4 mPadding{};
};

} // namespace Gui
