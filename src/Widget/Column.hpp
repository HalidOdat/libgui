#pragma once

#include "Core/Base.hpp"
#include "Widget/Container.hpp"

namespace Gui {

class Column : public Container {
public:
  using Handle = std::shared_ptr<Column>;

public:
  static Column::Handle create(Vec2 size = {0.0f, 0.0f});

  Vec2 layout(Constraints constraints) override;

public: // Do NOT use these function use the create functions!
  Column(Vec2 size)
    : Container(size)
  {}
};

} // namespace Gui
