#pragma once

#include <functional>
#include <optional>

#include "Widget/Constraints.hpp"
#include "Renderer/Renderer2D.hpp"

namespace Gui {

class Widget {
public:
    using Handle = std::shared_ptr<Widget>;
    using Visitor = std::function<bool(Widget*)>;

public:
    struct ClickEvent {
      Widget* target;
      Vec2 position;
      MouseButton button;
    };
    using ClickCallback = std::function<bool(ClickEvent)>;

public:
    virtual ~Widget() = default;

    virtual Vec2 layout(Constraints constraints) = 0;
    virtual void reportSize() const;
    virtual bool visit(Widget::Visitor& visitor) = 0;
    virtual void draw(Renderer2D& renderer) = 0;

    void setPosition(Vec2 position) { mPosition = position; }

    inline void setClickEventHandler(ClickCallback callback) { mClickCallback = callback; }
    inline void clearClickEventHandler() { mClickCallback = std::nullopt; }
    inline bool hasClickEventHandler() { return !!mClickCallback; }

    template<typename T>
    inline T* as() {
        if (!this) {
            return nullptr;
        }
        return dynamic_cast<T*>(this);
    }

    inline bool click(ClickEvent event) {
      if (mClickCallback) {
        return (*mClickCallback)(event);
      }
      return false;
    }
protected:
    Widget() = default;
    Widget(Vec2 size) : mSize{size} {}

public:
    Widget* parent = nullptr;

    Vec2 mPosition{};
    Vec2 mSize{};

    std::optional<ClickCallback> mClickCallback{};
};

} // namespace Gui
