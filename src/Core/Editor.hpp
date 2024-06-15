#pragma once

#include <string>
#include <vector>

namespace Gui {

  class Editor {
  public:
    struct Line {
        size_t begin;
        size_t end;
    };

  public:
    Editor(std::string text = "");

    const std::string& getText() const { return mData; }
    void setText(std::string text);

    void backspace();
    void deleteChar();
    size_t cursorRow() const;
    size_t cursorColumn() const;

    void moveLineUp();
    void moveLineDown();
    void moveCharLeft();
    void moveCharRight();
    void moveWordLeft();
    void moveWordRight();

    void moveToBegin();
    void moveToEnd();
    void moveToLineBegin();
    void moveToLineEnd();

    void moveParagraphUp();
    void moveParagraphDown();

    void insertChar(char x);
    void insertBuf(char *buf, size_t buf_len);
    void updateSelection(bool shift);
    void clipboardCopy();
    void clipboardPaste();

  private:
    void retokenize();

  private:
    std::string mData{};

    std::vector<Line> mLines{};
    bool mSelection{};
    size_t mSelectBegin{};
    size_t mCursor{};
    size_t mLastStroke{};
    std::string mClipboard{};
  };

}
