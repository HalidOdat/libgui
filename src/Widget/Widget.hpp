#pragma once

#include "Widget/Constraints.hpp"
#include "Renderer/Renderer2D.hpp"

#include <iostream>

namespace Gui {

class Widget {
public:
    using Handle = std::shared_ptr<Widget>;

public:
    virtual ~Widget() = default;

    virtual Vec2 layout(const Constraints& constraints) = 0;
    virtual void reportSize() const;
    virtual void draw(Renderer2D& renderer) = 0;

    void setPosition(Vec2 position) { mPosition = position; }

protected:
    Widget() = default;
    Widget(Vec2 size) : mSize{size} {}

protected:
    Vec2 mPosition{};
    Vec2 mSize{};
};

} // namespace Gui
