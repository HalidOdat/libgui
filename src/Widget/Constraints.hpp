#pragma once

namespace Gui {

class Constraints {
public:
  float minWidth;
  float minHeight;
  float maxWidth;
  float maxHeight;

  Constraints(float minWidth, float minHeight, float maxWidth, float maxHeight)
    : minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight)
  {}
};

} // namespace Gui
