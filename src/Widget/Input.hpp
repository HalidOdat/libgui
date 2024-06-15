#pragma once

#include <functional>
#include <Events/MouseEvent.hpp>
#include "Widget/Widget.hpp"

namespace Gui {

class Input : public Widget {
public:
  using Handle   = std::shared_ptr<Input>;
  using OnChangeCallback = std::function<void(std::string& text)>;

public:

  enum class Type {
    None,
    Alpha,
    Numeric,
  };

public:
  static Input::Handle create(OnChangeCallback callback, std::string text = "", float fontSize = 28);

  Vec2 layout(Constraints constraints) override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Handle self, Widget::Visitor& visitor) override { return visitor(self); }

  void setFontSize(float size) { mFontSize = size; }

  static Input::Handle deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors);

  const std::string& getText() const { return mText; }
  void setText(std::string value) { mText = std::move(value); }
  Type getType() const { return mType; }
  void setType(Type value) { mType = value; }

  void setOnChange(OnChangeCallback onChange) { mOnChange = std::move(onChange); }

  void setHint(std::string value) { mHint = value; }
  const std::string& getHint() const { return mHint; }

  void setColor(Vec4 value) { mColor = value; }
  Vec4 getColor() const { return mColor; }

public: // Do NOT use these function use the create functions!
  Input(OnChangeCallback callback, std::string text, float fontSize)
    : mOnChange{callback}, mText{std::move(text)}, mFontSize{fontSize}
  {}

private:
  OnChangeCallback mOnChange;
  std::string mText{};
  std::string mHint{};
  float mFontSize;
  Type mType = Type::None;

  Vec4 mColor = Color::BLACK;
};

} // namespace Gui
