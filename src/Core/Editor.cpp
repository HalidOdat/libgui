#include "Core/Editor.hpp"
#include <cstring>
#include <cassert>
#include <cctype>

// Inspired by https://github.com/tsoding/ded
// LICENSE: MIT

namespace Gui {

  Editor::Editor(std::string text)
    : mData(std::move(text))
  {
    retokenize();
  }

  void Editor::setText(std::string text) {
    mData = std::move(text);
    mCursor = 0;
    retokenize();
  }

  void Editor::retokenize() {
    mLines.clear();

    Line line;
    line.begin = 0;

    for (size_t i = 0; i < mData.size(); ++i) {
        if (mData[i] == '\n') {
            line.end = i;
            mLines.push_back(line);
            line.begin = i + 1;
        }
    }

    line.end = mData.size();
    mLines.push_back(line);
  }

  void Editor::backspace() {
    if (mCursor > mData.size()) {
        mCursor = mData.size();
    }
    if (mCursor == 0) return;

    memmove(
        &mData[mCursor - 1],
        &mData[mCursor],
        mData.size() - mCursor
    );

    mCursor -= 1;
    mData.pop_back();

    retokenize();
  }

  void Editor::deleteChar() {
      if (mCursor >= mData.size()) return;
      memmove(
          &mData[mCursor],
          &mData[mCursor + 1],
          mData.size() - mCursor - 1
      );
      mData.pop_back();
      retokenize();
  }

  size_t Editor::cursorRow() const {
      assert(mLines.size() > 0);
      for (size_t row = 0; row < mLines.size(); ++row) {
          Line line = mLines[row];
          if (line.begin <= mCursor && mCursor <= line.end) {
              return row;
          }
      }
      return mLines.size() - 1;
  }

  size_t Editor::cursorColumn() const {
    size_t cursor_row = cursorRow();
    Line line = mLines[cursor_row];
    size_t cursor_col = mCursor - line.begin;
    return cursor_col;
  }

  void Editor::moveLineUp() {
      size_t cursor_row = cursorRow();
      size_t cursor_col = mCursor - mLines[cursor_row].begin;
      if (cursor_row > 0) {
          Line next_line = mLines[cursor_row - 1];
          size_t next_line_size = next_line.end - next_line.begin;
          if (cursor_col > next_line_size) cursor_col = next_line_size;
          mCursor = next_line.begin + cursor_col;
      }
  }

  void Editor::moveLineDown() {
      size_t cursor_row = cursorRow();
      size_t cursor_col = mCursor - mLines[cursor_row].begin;
      if (cursor_row < mLines.size() - 1) {
          Line next_line = mLines[cursor_row + 1];
          size_t next_line_size = next_line.end - next_line.begin;
          if (cursor_col > next_line_size) cursor_col = next_line_size;
          mCursor = next_line.begin + cursor_col;
      }
  }


  void Editor::moveCharLeft() {
    if (mCursor > 0) mCursor -= 1;
  }

  void Editor::moveCharRight() {
    if (mCursor < mData.size()) mCursor += 1;
  }

  void Editor::insertChar(char x) {
    insertBuf(&x, 1);
  }

  void Editor::insertBuf(char *buf, size_t buf_len) {
    if (mCursor > mData.size()) {
        mCursor = mData.size();
    }

    for (size_t i = 0; i < buf_len; ++i) {
        // da_append(&e->data, '\0');
        mData.push_back('x');
    }
    memmove(
        &mData[mCursor + buf_len],
        &mData[mCursor],
        mData.size() - mCursor - buf_len
    );
    memcpy(&mData[mCursor], buf, buf_len);
    mCursor += buf_len;

    retokenize();
  }

  void Editor::moveWordLeft() {
      while (mCursor > 0 && !isalnum(mData[mCursor - 1])) {
          mCursor -= 1;
      }
      while (mCursor > 0 && isalnum(mData[mCursor - 1])) {
          mCursor -= 1;
      }
  }

  void Editor::moveWordRight() {
      while (mCursor < mData.size() && !isalnum(mData[mCursor])) {
          mCursor += 1;
      }
      while (mCursor < mData.size() && isalnum(mData[mCursor])) {
          mCursor += 1;
      }
  }

  void Editor::moveToLineBegin() {
      size_t row = cursorRow();
      mCursor = mLines[row].begin;
  }

  void Editor::moveToLineEnd() {
      size_t row = cursorRow();
      mCursor = mLines[row].end;
  }

}