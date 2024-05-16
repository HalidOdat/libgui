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
  static Input::Handle create(OnChangeCallback callback, std::string text = "", float fontSize = 28);

  Vec2 layout(Constraints constraints) override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Visitor& visitor) override { return visitor(this); }

  void setFontSize(float size) { mFontSize = size; }

public: // Do NOT use these function use the create functions!
  Input(OnChangeCallback callback, std::string text, float fontSize)
    : mOnChange{callback}, mText{std::move(text)}, mFontSize{fontSize}
  {}

private:
  OnChangeCallback mOnChange;
  std::string mText;
  float mFontSize;
};

} // namespace Gui