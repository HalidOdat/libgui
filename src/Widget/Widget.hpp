#pragma once

#include <functional>

#include "Widget/Constraints.hpp"
#include "Renderer/Renderer2D.hpp"

namespace Gui {

class Widget {
public:
    using Handle = std::shared_ptr<Widget>;
    using Visitor = std::function<bool(Widget*)>;

public:
    virtual ~Widget() = default;

    virtual Vec2 layout(Constraints constraints) = 0;
    virtual void reportSize() const;
    virtual bool visit(Widget::Visitor& visitor) = 0;
    virtual void draw(Renderer2D& renderer) = 0;

    void setPosition(Vec2 position) { mPosition = position; }

    template<typename T>
    T* as() { return dynamic_cast<T*>(this); }
protected:
    Widget() = default;
    Widget(Vec2 size) : mSize{size} {}

public:
    Widget* parent = nullptr;

    Vec2 mPosition{};
    Vec2 mSize{};
};

} // namespace Gui
