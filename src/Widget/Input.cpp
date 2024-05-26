#include "Widget/Input.hpp"
#include <Core/Color.hpp>
#include <cctype>

namespace Gui {

Input::Handle Input::create(OnChangeCallback callback, std::string text, float fontSize) {
  auto target = std::make_shared<Input>(std::move(callback), std::move(text), fontSize);
  target->mFocusable = true;
  target->addKeyEventHandler([=](auto event){
    if (!target->mFocused) {
      return false;
    }

    if (event.type != Widget::KeyEventType::Pressed) {
      return false;
    }

    if (event.key == Key::Backspace) {
      if (!target->mText.empty()) {
        target->mText.pop_back();
        target->mOnChange(target->mText);
      }
      return true;
    }

    auto ch = getKeyChar(event.key, event.modifier);
    if (!ch) {
      return false;
    }

    target->mText.push_back(ch);
    target->mOnChange(target->mText);
    return true;
  });
  return target;
}

Vec2 Input::layout(Constraints constraints) {
  mSize.x = constraints.maxWidth;
  mSize.y = std::max(constraints.minHeight, std::min(constraints.maxHeight, mFontSize * 2.0f));
  return mSize;
}

void Input::draw(Renderer2D& renderer) {
  renderer.drawQuad(
    mPosition,
    mSize,
    mFocused ? rgba(0xAA2222FF) : rgba(0x222222FF),
    Effect::Type::Rounded
  );
  renderer.drawText(mText, mPosition + mFontSize/2.0f, mFontSize, Color::BLACK);
  if (mFocused) {
    renderer.drawQuad(
      (mPosition + mFontSize/2.0f) + Vec2{(mFontSize - mFontSize/7.0f) * mText.size(), 0.0f},
      Vec2{mFontSize*0.2, mFontSize},
      rgba(0x222222FF)
    );
  }
}

Input::Handle Input::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  auto id = deserializeId(node["id"], errors);

  std::string text;
  if (node.IsMap() && node["text"] && node["text"].IsScalar()) {
    text = node["text"].as<std::string>();
  }

  float fontSize = 28;
  if (node.IsMap() && node["font-size"] && node["font-size"].IsScalar()) {
    fontSize = node["font-size"].as<float>();
  }

  auto result = Input::create([](auto& _text){}, text, fontSize);
  result->setId(id);
  return result;
}

} // namespace Gui
