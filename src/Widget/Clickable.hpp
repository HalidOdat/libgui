#pragma once

#include <functional>
#include "Widget/Widget.hpp"

namespace Gui {

class Clickable : public Widget {
public:
  using Handle   = std::shared_ptr<Clickable>;
  using Callback = std::function<void(Widget::Handle)>;

public:
  static Clickable::Handle create(Widget::Handle child, Callback callback);

  Vec2 layout(const Constraints& constraints) override;
  void draw(Renderer2D& renderer) override;

public: // Do NOT use these function use the create functions!
  Clickable(Widget::Handle child, Callback callback)
    : mChild{child}, mCallback{callback}
  {}

private:
  Widget::Handle mChild;
  Callback mCallback;
};

} // namespace Gui
