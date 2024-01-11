#pragma once

#include "Core/Macro.hpp"

namespace Gui {

  class Logger {
  public:
#ifdef GUI_DEBUG_MODE
    static void trace(const char *format, ...);
    static void info(const char* format, ...);
#else
    static void trace(const char *format, ...) { (void)format; }
    static void info(const char* format, ...) { (void)format; }
#endif
    static void warn(const char* format, ...);
    static void error(const char* format, ...);
  };

} // namespace Gui