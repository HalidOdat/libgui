#include "Widget/TextArea.hpp"
#include <Core/Color.hpp>
#include <cctype>

namespace Gui {

TextArea::Handle TextArea::create(OnChangeCallback callback, std::string text, float fontSize) {
  auto target = std::make_shared<TextArea>(std::move(callback), std::move(text), fontSize);  
  target->mFocusable = true;
  target->addKeyEventHandler([=](auto event){
    if (!target->mFocused) {
      return false;
    }

    if (event.type != Widget::KeyEventType::Pressed) {
      return false;
    }

    if (event.key == Key::Backspace) {
      if (!target->getText().empty()) {
        target->mEditor.backspace();
        target->mOnChange(target->getText());
      }
      return true;
    } else if (event.key == Key::Enter) {
      target->mEditor.insertChar('\n');
      target->mOnChange(target->getText());
      return true;
    } else if (event.key == Key::Up) {
      target->mEditor.moveLineUp();
    } else if (event.key == Key::Down) {
      target->mEditor.moveLineDown();
    } else if (event.key == Key::Right) {
      if (event.modifier == KeyModifier::Control) {
        target->mEditor.moveWordRight();
      } else {
        target->mEditor.moveCharRight();
      }
    } else if (event.key == Key::Left) {
      if (event.modifier == KeyModifier::Control) {
        target->mEditor.moveWordLeft();
      } else {
        target->mEditor.moveCharLeft();
      }
    } else if (event.key == Key::Tab) {
      target->mEditor.insertBuf("  ", 2);
    } else if (event.key == Key::Home) {
      target->mEditor.moveToLineBegin();
    } else if (event.key == Key::End) {
      target->mEditor.moveToLineEnd();
    }

    auto ch = getKeyChar(event.key, event.modifier);
    if (!ch) {
      return false;
    }

    target->mEditor.insertChar(ch);
    target->mOnChange(target->getText());
    return true;
  });
  return target;
}

Vec2 TextArea::layout(Constraints constraints) {
  mSize.x = constraints.maxWidth;
  mSize.y = constraints.maxHeight;
  return mSize;
}

void TextArea::draw(Renderer2D& renderer) {
  renderer.drawQuad(
    mPosition,
    mSize,
    mFocused ? rgba(0xAA2222FF) : rgba(0x222222FF),
    Effect::Type::Rounded
  );

  auto row    = mEditor.cursorRow();
  auto column = mEditor.cursorColumn();

  renderer.drawText(mEditor.getText(), mPosition + mFontSize/2.0f, mFontSize, Color::BLACK);
  if (mFocused) {
    renderer.drawQuad(
      (mPosition + mFontSize/2.0f) + Vec2{(mFontSize - mFontSize/7.0f) * column, mFontSize * row},
      Vec2{mFontSize*0.2, mFontSize},
      rgba(0x222222FF)
    );
  }
}

TextArea::Handle TextArea::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  auto id = deserializeId(node["id"], errors);

  std::string text;
  if (node.IsMap() && node["text"] && node["text"].IsScalar()) {
    text = node["text"].as<std::string>();
  }

  float fontSize = 28.0f;
  if (node.IsMap() && node["font-size"] && node["font-size"].IsScalar()) {
    fontSize = node["font-size"].as<float>();
  }

  auto result = TextArea::create([](auto&){}, text, fontSize);
  result->setId(id);
  return result;
}

void TextArea::setText(std::string value) {
  mEditor.setText(std::move(value));
  mOnChange(mEditor.getText());
}

} // namespace Gui
