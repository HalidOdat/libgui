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
    using Visitor = std::function<bool(Widget::Handle)>;

public:
    struct ClickEvent {
      Widget::Handle target;
      Vec2 position;
      MouseButton button;
    };
    using ClickCallback = std::function<bool(ClickEvent)>;

    enum class KeyEventType {
        Pressed,
        Released,
    };

    struct KeyEvent {
      Widget::Handle target;
      Key key;
      KeyEventType type;
      KeyModifier modifier;
    };
    using KeyCallback = std::function<bool(KeyEvent)>;

public:
    virtual ~Widget() = default;

    virtual Vec2 layout(Constraints constraints) = 0;
    virtual void reportSize() const;
    virtual bool visit(Widget::Handle self, Widget::Visitor& visitor) = 0;
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

    inline bool isFocusable() const { return mFocusable; }
    inline const std::string& getId() const { return mId; }
    inline void setId(std::string id) { mId = id; }
protected:
    Widget() = default;
    Widget(Vec2 size) : mSize{size} {}

public:
    Widget* parent = nullptr;
    std::string mId;

    Vec2 mPosition{};
    Vec2 mSize{};

    bool mFocusable = false;
    bool mFocused = false;

    std::vector<ClickCallback> mClickCallbacks{};
    std::vector<KeyCallback> mKeyCallbacks{};
};

} // namespace Gui
