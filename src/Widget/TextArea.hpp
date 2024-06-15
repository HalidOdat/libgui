#pragma once

#include <functional>
#include <Events/MouseEvent.hpp>
#include "Widget/Widget.hpp"
#include "Core/Editor.hpp"

namespace Gui {

class TextArea : public Widget {
public:
  using Handle   = std::shared_ptr<TextArea>;
  using OnChangeCallback = std::function<void(const std::string& text)>;

public:
  static TextArea::Handle create(OnChangeCallback callback, std::string text = "", float fontSize = 28);

  Vec2 layout(Constraints constraints) override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Handle self, Widget::Visitor& visitor) override { return visitor(self); }

  void setFontSize(float size) { mFontSize = size; }

  static TextArea::Handle deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors);

  const std::string& getText() { return mEditor.getText(); }
  void setText(std::string value);
  void setOnChange(OnChangeCallback onChange) { mOnChange = std::move(onChange); }

public: // Do NOT use these function use the create functions!
  TextArea(OnChangeCallback callback, std::string text, float fontSize)
    : mOnChange{callback}, mEditor{std::move(text)}, mFontSize{fontSize}
  {}

private:
  OnChangeCallback mOnChange;
  Editor mEditor;
  float mFontSize;
};

} // namespace Gui
