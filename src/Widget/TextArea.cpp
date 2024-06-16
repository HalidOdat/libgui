#include "Widget/TextArea.hpp"
#include <Core/Color.hpp>
#include <cctype>
#include <numeric>
#include <utility>

namespace Gui {

static std::pair<size_t, size_t> count(const std::string& s) {
  auto maxColumn = 0;
  auto column = 0;
  auto lines = std::accumulate(
    s.cbegin(),
    s.cend(),
    0,
    [&](int prev, char c) {
      maxColumn = std::max(maxColumn, column);
      if (c == '\n') {
        column = 0;
      }
      column++;

      return c != '\n' ? prev : prev + 1;
    }
  ) + 1;

  return std::make_pair(lines, maxColumn);
}

TextArea::Handle TextArea::create(OnChangeCallback callback, std::string text, float fontSize) {
  auto target = std::make_shared<TextArea>(std::move(callback), std::move(text), fontSize);  
  auto[lines, maxColumn] = count(target->mEditor.getText());
  target->mLines = lines;
  target->mMaxColumn = maxColumn;
  
  target->mFixedHeightSizeWidget = target->mFitContent;
  target->mFixedWidthSizeWidget  = target->mFitContent;

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
  if (mFitContent) {
    mSize.x = (mFontSize - mFontSize/7.0f) * mMaxColumn;
    mSize.y = (mFontSize)                  * mLines;
  } else {
    mSize.x = constraints.maxWidth;
    mSize.y = constraints.maxHeight;
  }
  return mSize;
}

void TextArea::draw(Renderer2D& renderer) {
  Vec2 offset = Vec2{4.0f};
  if (mFocused) {
    renderer.drawQuad(mPosition, mSize, rgba(0xAA2222FF));
    renderer.drawQuad(mPosition + offset, mSize - offset * 2.0f, mBackground);
  } else {
    renderer.drawQuad(mPosition, mSize, mBackground);
  }

    // mFocused ? rgba(0xAA2222FF) : rgba(0x222222FF),

  auto row    = mEditor.cursorRow();
  auto column = mEditor.cursorColumn();

  renderer.drawText(mEditor.getText(), mPosition + mFontSize/2.0f + offset, mFontSize, mColor);
  if (mFocused) {
    renderer.drawQuad(
      (mPosition + mFontSize/2.0f) + Vec2{(mFontSize - mFontSize/7.0f) * column, mFontSize * row} + offset,
      Vec2{mFontSize*0.2, mFontSize},
      mColor
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

  bool editable = true;
  if (node.IsMap() && node["editable"] && node["editable"].IsScalar()) {
    editable = node["editable"].as<bool>();
  }

  bool fitContent = false;
  if (node.IsMap() && node["fit-content"] && node["fit-content"].IsScalar()) {
    fitContent = node["fit-content"].as<bool>();
  }

  bool display = true;
  if (node.IsMap() && node["display"] && node["display"].IsScalar()) {
    display = node["display"].as<bool>();
  }

  auto color = Color::BLACK;
  if (node["color"]) {
    color = deserializeColor(node["color"], errors);
  }

  auto background = Color::WHITE;
  if (node["background"]) {
    background = deserializeColor(node["background"], errors);
  }

  auto result = TextArea::create([](auto&){}, text, fontSize);
  result->setId(id);
  result->mFocusable = editable;
  result->setFitContent(fitContent);
  result->setDisplay(display);
  result->setBackground(background);
  result->setColor(color);
  return result;
}

void TextArea::setText(std::string value) {
  auto[lines, maxColumn] = count(value);
  mLines = lines;
  mMaxColumn = maxColumn;

  mEditor.setText(std::move(value));
  mOnChange(mEditor.getText());
}

void TextArea::setFitContent(bool value) {
  mFixedWidthSizeWidget = value;
  mFixedHeightSizeWidget = value;
  mFitContent = value;
}

} // namespace Gui
