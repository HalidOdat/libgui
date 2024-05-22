#pragma once

#include "Core/Base.hpp"
#include "Widget/Container.hpp"

namespace Gui {

class Row : public Container {
public:
  using Handle = std::shared_ptr<Row>;

public:
  static Row::Handle create(Vec2 size = {0.0f, 0.0f});

  static Row::Handle deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors);
public: // Do NOT use these function use the create functions!
  Row(Vec2 size)
    : Container(size)
  {}
};

} // namespace Gui
