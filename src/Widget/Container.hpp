#pragma once

#include "Core/Base.hpp"
#include "Widget/Widget.hpp"

namespace Gui {

enum class MainAxis {
  Start,
  End,
  Center,
};

enum class CrossAxis {
  Start,
  End,
  Center,
};

enum class Alignment {
  None,
  Horizontal,
  Vertical,
  Center,
};

class Container : public Widget {
public:
  using Handle = std::shared_ptr<Container>;

public:
  static Container::Handle create(Vec2 size = {0.0f, 0.0f});

  void addChild(Widget::Handle child);
  void setColor(Vec4 color) { mColor = color; }
  void clearChildren() { mChildren.clear(); }
  void setPadding(Vec4 padding) { mPadding = padding; }
  void setAlignment(Alignment alignment) { mAlignment = alignment; }
  inline void setWidth(float size) { mWidth = size; }
  inline float getWidth() const { return mWidth; }
  inline void setHeight(float size) { mHeight = size; }
  inline float getHeight() const { return mHeight; }
  inline void setMainAxis(MainAxis value) { mMainAxis = value; }
  inline MainAxis getMainAxis() const { return mMainAxis; }
  inline void setCrossAxis(CrossAxis value) { mCrossAxis = value; }
  inline CrossAxis getCrossAxis() const { return mCrossAxis; }

  Vec2 layout(Constraints constraints) override;
  void reportSize() const override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Handle self, Widget::Visitor& visitor) override {
    for (auto child : mChildren) {
      if (!child->visit(child, visitor)) {
        return false;
      }
    }
    return visitor(self);
  }

  static Container::Handle deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors);

public: // Do NOT use these function use the create functions!
  Container(Vec2 size)
    : Widget(size)
  {}

protected:
  std::vector<Widget::Handle> mChildren;
  Vec4 mColor{1.0f, 1.0f, 1.0f, 0.0f};

  Vec4 mPadding{};
  Alignment mAlignment{Alignment::None};

  float mWidth{INFINITY};
  float mHeight{INFINITY};

  MainAxis mMainAxis = MainAxis::Center;
  CrossAxis mCrossAxis = CrossAxis::Center;
};

} // namespace Gui
