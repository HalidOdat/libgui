#pragma once

#include <Events/MouseEvent.hpp>
#include "Widget/Widget.hpp"
#include <cmath>

namespace Gui {

class CheckBox : public Widget {
public:
  using Handle = std::shared_ptr<CheckBox>;
  using OnChangeCallback = std::function<void(bool value)>;

public:
  static CheckBox::Handle create(OnChangeCallback callback);

  Vec2 layout(Constraints constraints) override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Handle self, Widget::Visitor& visitor) override { return visitor(self); }

  inline void setColor(Vec4 color) { mColor = color; }
  inline Vec4 getColor() const { return mColor; }
  inline void setBackground(Vec4 color) { mBackground = color; }
  inline Vec4 getBackground() const { return mBackground; }
  inline void setBorderColor(Vec4 color) { mBorderColor = color; }
  inline Vec4 getBorderColor() const { return mBorderColor; }
  inline void setMargin(Vec4 color) { mMargin = color; }
  inline Vec4 getMargin() const { return mMargin; }
  inline void setWidth(float size) { mWidth = size; }
  inline float getWidth() const { return mWidth; }
  inline void setHeight(float size) { mHeight = size; }
  inline float getHeight() const { return mHeight; }
  void setOnChange(OnChangeCallback onChange) { mOnChange = std::move(onChange); }

  static CheckBox::Handle deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors);

public: // Do NOT use these function use the create functions!
  CheckBox(OnChangeCallback callback)
    : mOnChange{std::move(callback)}
  {}

private:
  OnChangeCallback mOnChange;

  bool mValue = false;
  Vec4 mColor = Color::WHITE;
  Vec4 mBackground = Color::BLACK;
  Vec4 mBorderColor = Color::DARK_GRAY;
  Vec4 mMargin{};

  float mWidth{40.0f};
  float mHeight{40.0f};
};

} // namespace Gui
