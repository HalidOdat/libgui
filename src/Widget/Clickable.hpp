#pragma once

#include <functional>
#include <Events/MouseEvent.hpp>
#include "Widget/Widget.hpp"

namespace Gui {

class Clickable : public Widget {
public:
  struct Event {
    Clickable* parent;
    Widget::Handle target;
    Vec2 position;
    MouseButton button;
  };

public:
  using Handle   = std::shared_ptr<Clickable>;
  using Callback = std::function<bool(Clickable::Event)>;

public:
  static Clickable::Handle create(Widget::Handle child, Callback callback);

  inline Widget::Handle getChild() { return mChild; }

  Vec2 layout(Constraints constraints) override;
  void draw(Renderer2D& renderer) override;
  bool visit(Widget::Visitor& visitor) override {
    if (!mChild->visit(visitor)) {
      return false;
    }
    return visitor(this);
  }

  bool call(Clickable::Event event) {
    return mCallback(event);
  }

public: // Do NOT use these function use the create functions!
  Clickable(Widget::Handle child, Callback callback)
    : mChild{child}, mCallback{callback}
  {}

private:
  Widget::Handle mChild;
  Callback mCallback;
};

} // namespace Gui
