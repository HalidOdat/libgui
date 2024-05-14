#pragma once

#include <functional>
#include <vector>

#include "Widget/Constraints.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Events/KeyEvent.hpp"

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

    enum class KeyEventType {
        Pressed,
        Released,
    };

    struct KeyEvent {
      Widget* target;
      Key key;
      KeyEventType type;
    };
    using KeyCallback = std::function<bool(KeyEvent)>;

public:
    virtual ~Widget() = default;

    virtual Vec2 layout(Constraints constraints) = 0;
    virtual void reportSize() const;
    virtual bool visit(Widget::Visitor& visitor) = 0;
    virtual void draw(Renderer2D& renderer) = 0;

    void setPosition(Vec2 position) { mPosition = position; }

    inline void addClickEventHandler(ClickCallback callback) { mClickCallbacks.push_back(callback); }
    inline void clearClickEventHandlers() { mClickCallbacks.clear(); }
    inline bool hasClickEventHandler() { return !mClickCallbacks.empty(); }

    inline void addKeyEventHandler(KeyCallback callback) { mKeyCallbacks.push_back(callback); }
    inline void clearKeyEventHandlers() { mKeyCallbacks.clear(); }
    inline bool hasKeyEventHandler() { return !mKeyCallbacks.empty(); }

    template<typename T>
    inline T* as() { return dynamic_cast<T*>(this); }

    inline bool click(ClickEvent event) {
      bool handled = false;
      for (auto& handler : mClickCallbacks) {
        handled = handled || handler(event);
      }
      return handled;
    }

    inline bool triggerKeyEvent(KeyEvent event) {
      bool handled = false;
      for (auto& handler : mKeyCallbacks) {
        handled = handled || handler(event);
      }
      return handled;
    }
protected:
    Widget() = default;
    Widget(Vec2 size) : mSize{size} {}

public:
    Widget* parent = nullptr;

    Vec2 mPosition{};
    Vec2 mSize{};

    std::vector<ClickCallback> mClickCallbacks{};
    std::vector<KeyCallback> mKeyCallbacks{};
};

} // namespace Gui
