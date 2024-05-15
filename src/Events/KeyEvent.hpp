#pragma once

#include "Core/Type.hpp"
#include "Events/Event.hpp"

namespace Gui {

  // Taken from glfw3.h header
  enum class Key {
    Unknown            = -1,
    Space              = 32,
    Apostrophe         = 39,  /* ' */
    Comma              = 44,  /* , */
    Minus              = 45,  /* - */
    Period             = 46,  /* . */
    Slash              = 47,  /* / */
    _0                 = 48,
    _1                 = 49,
    _2                 = 50,
    _3                 = 51,
    _4                 = 52,
    _5                 = 53,
    _6                 = 54,
    _7                 = 55,
    _8                 = 56,
    _9                 = 57,
    Semicolon          = 59,  /* ; */
    Equal              = 61,  /* = */
    A                  = 65,
    B                  = 66,
    C                  = 67,
    D                  = 68,
    E                  = 69,
    F                  = 70,
    G                  = 71,
    H                  = 72,
    I                  = 73,
    J                  = 74,
    K                  = 75,
    L                  = 76,
    M                  = 77,
    N                  = 78,
    O                  = 79,
    P                  = 80,
    Q                  = 81,
    R                  = 82,
    S                  = 83,
    T                  = 84,
    U                  = 85,
    V                  = 86,
    W                  = 87,
    X                  = 88,
    Y                  = 89,
    Z                  = 90,
    LeftBracket        = 91,  /* [ = */
    Backslash          = 92,  /* \ = */
    RightBracket       = 93,  /* ] = */
    GraveAccent        = 96,  /* ` = */
    World_1            = 161, /* non-US #1 = */
    World_2            = 162, /* non-US #2 = */
    Escape             = 256,
    Enter              = 257,
    Tab                = 258,
    Backspace          = 259,
    Insert             = 260,
    Delete             = 261,
    Right              = 262,
    Left               = 263,
    Down               = 264,
    Up                 = 265,
    PageUp             = 266,
    PageDown           = 267,
    Home               = 268,
    End                = 269,
    CapsLock           = 280,
    ScrollLock         = 281,
    NumLock            = 282,
    PrintScreen        = 283,
    Pause              = 284,
    F1                 = 290,
    F2                 = 291,
    F3                 = 292,
    F4                 = 293,
    F5                 = 294,
    F6                 = 295,
    F7                 = 296,
    F8                 = 297,
    F9                 = 298,
    F10                = 299,
    F11                = 300,
    F12                = 301,
    F13                = 302,
    F14                = 303,
    F15                = 304,
    F16                = 305,
    F17                = 306,
    F18                = 307,
    F19                = 308,
    F20                = 309,
    F21                = 310,
    F22                = 311,
    F23                = 312,
    F24                = 313,
    F25                = 314,
    Kp_0               = 320,
    Kp_1               = 321,
    Kp_2               = 322,
    Kp_3               = 323,
    Kp_4               = 324,
    Kp_5               = 325,
    Kp_6               = 326,
    Kp_7               = 327,
    Kp_8               = 328,
    Kp_9               = 329,
    KpDecimal          = 330,
    KpDivide           = 331,
    KpMultiply         = 332,
    KpSubtract         = 333,
    KpAdd              = 334,
    KpEnter            = 335,
    KpEqual            = 336,
    LeftShift          = 340,
    LeftControl        = 341,
    LeftAlt            = 342,
    LeftSuper          = 343,
    RightShift         = 344,
    RightControl       = 345,
    RightAlt           = 346,
    RightSuper         = 347,
    Menu               = 348,
  };

  enum class KeyModifier : u8 {
    None     = 0x0000,
    Shift    = 0x0001,
    Control  = 0x0002,
    Alt      = 0x0004,
    Super    = 0x0008,
    CapsLock = 0x0010,
    NumsLock = 0x0020,
  };

  inline KeyModifier operator|(KeyModifier lhs, KeyModifier rhs) {
    return static_cast<KeyModifier>(static_cast<u8>(lhs) | static_cast<u8>(rhs));
  }
  inline KeyModifier operator&(KeyModifier lhs, KeyModifier rhs) {
    return static_cast<KeyModifier>(static_cast<u8>(lhs) & static_cast<u8>(rhs));
  }

  inline char getKeyChar(Key key, KeyModifier modifier) {
    switch (key) {
      case Key::Space:        return ' ';
      case Key::Apostrophe:   return modifier == KeyModifier::Shift ? '"' : '\'';
      case Key::Comma:        return modifier == KeyModifier::Shift ? '<' : ',';
      case Key::Minus:        return modifier == KeyModifier::Shift ? '_' : '-';
      case Key::Period:       return modifier == KeyModifier::Shift ? '>' : '.';
      case Key::Slash:        return modifier == KeyModifier::Shift ? '?' : '/';
      case Key::_0:           return modifier == KeyModifier::Shift ? ')' : '0';
      case Key::_1:           return modifier == KeyModifier::Shift ? '!' : '1';
      case Key::_2:           return modifier == KeyModifier::Shift ? '@' : '2';
      case Key::_3:           return modifier == KeyModifier::Shift ? '#' : '3';
      case Key::_4:           return modifier == KeyModifier::Shift ? '$' : '4';
      case Key::_5:           return modifier == KeyModifier::Shift ? '%' : '5';
      case Key::_6:           return modifier == KeyModifier::Shift ? '^' : '6';
      case Key::_7:           return modifier == KeyModifier::Shift ? '&' : '7';
      case Key::_8:           return modifier == KeyModifier::Shift ? '*' : '8';
      case Key::_9:           return modifier == KeyModifier::Shift ? '(' : '9';
      case Key::Semicolon:    return modifier == KeyModifier::Shift ? ':' : ';';
      case Key::Equal:        return modifier == KeyModifier::Shift ? '+' : '=';
      case Key::A:            return modifier == KeyModifier::Shift ? 'A' : 'a';
      case Key::B:            return modifier == KeyModifier::Shift ? 'B' : 'b';
      case Key::C:            return modifier == KeyModifier::Shift ? 'C' : 'c';
      case Key::D:            return modifier == KeyModifier::Shift ? 'D' : 'd';
      case Key::E:            return modifier == KeyModifier::Shift ? 'E' : 'e';
      case Key::F:            return modifier == KeyModifier::Shift ? 'F' : 'f';
      case Key::G:            return modifier == KeyModifier::Shift ? 'G' : 'g';
      case Key::H:            return modifier == KeyModifier::Shift ? 'H' : 'h';
      case Key::I:            return modifier == KeyModifier::Shift ? 'I' : 'i';
      case Key::J:            return modifier == KeyModifier::Shift ? 'J' : 'j';
      case Key::K:            return modifier == KeyModifier::Shift ? 'K' : 'k';
      case Key::L:            return modifier == KeyModifier::Shift ? 'L' : 'l';
      case Key::M:            return modifier == KeyModifier::Shift ? 'M' : 'm';
      case Key::N:            return modifier == KeyModifier::Shift ? 'N' : 'n';
      case Key::O:            return modifier == KeyModifier::Shift ? 'O' : 'o';
      case Key::P:            return modifier == KeyModifier::Shift ? 'P' : 'p';
      case Key::Q:            return modifier == KeyModifier::Shift ? 'Q' : 'q';
      case Key::R:            return modifier == KeyModifier::Shift ? 'R' : 'r';
      case Key::S:            return modifier == KeyModifier::Shift ? 'S' : 's';
      case Key::T:            return modifier == KeyModifier::Shift ? 'T' : 't';
      case Key::U:            return modifier == KeyModifier::Shift ? 'U' : 'u';
      case Key::V:            return modifier == KeyModifier::Shift ? 'V' : 'v';
      case Key::W:            return modifier == KeyModifier::Shift ? 'W' : 'w';
      case Key::X:            return modifier == KeyModifier::Shift ? 'X' : 'x';
      case Key::Y:            return modifier == KeyModifier::Shift ? 'Y' : 'y';
      case Key::Z:            return modifier == KeyModifier::Shift ? 'Z' : 'z';
      case Key::LeftBracket:  return modifier == KeyModifier::Shift ? '{' : '[';
      case Key::Backslash:    return modifier == KeyModifier::Shift ? '|' : '\\';
      case Key::RightBracket: return modifier == KeyModifier::Shift ? '}' : ']';
      case Key::GraveAccent:  return modifier == KeyModifier::Shift ? '~' : '`';
      default: return 0;
    }
  }

  class KeyEvent : public Event {
  public:
    Key getKey()      const { return this->mKey; }
    i32 getKeyCode()  const { return (i32)this->mKey; }

  protected:
    KeyEvent(Event::Type type, Key key)
      : Event{type}, mKey{key}
    {}

  protected:
    Key mKey;
  };

  class KeyPressedEvent : public KeyEvent {    
  public:
    static constexpr const auto TYPE = Event::Type::KeyPressed;

  public:
    KeyPressedEvent(Key key, KeyModifier modifier, bool repeat)
      : KeyEvent(TYPE, key), mModifier{modifier}, mRepeat{repeat}
    {}

    inline KeyModifier getModifier() const { return this->mModifier; }
    inline bool isRepeat() const { return this->mRepeat; }

  private:
    KeyModifier mModifier;
    bool mRepeat = false;
  };

  class KeyReleasedEvent : public KeyEvent {
  public:
    static constexpr const auto TYPE = Event::Type::KeyReleased;

  public:
    KeyReleasedEvent(Key key)
      : KeyEvent(TYPE, key)
    {}
  };

} // namespace Gui