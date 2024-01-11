#pragma once

#include <cstdio>
#include <cstdlib>

#if defined(__EMSCRIPTEN__)
  #define GUI_PLATFORM_WEB 1
#else
  #define GUI_PLATFORM_NATIVE 1
#endif

#if !defined(GUI_DEBUG_MODE) && !defined(GUI_RELEASE_MODE)
# define GUI_DEBUG_MODE 1
#endif

#define GUI_ASSERT_IMPL(type, cond, sep, msg) do { if (!(cond)) { fprintf(stderr, "%s:%d: " type " Failed '%s'%s%s\n", __FILE__, __LINE__, #cond, sep, msg); fflush(stdout); fflush(stderr); std::exit(1); } } while(false)

#ifdef GUI_DEBUG_MODE
# define GUI_DEBUG_ASSERT(cond)                   GUI_ASSERT_IMPL("Debug Assert", cond, "", "")
# define GUI_DEBUG_ASSERT_WITH_MESSAGE(cond, msg) GUI_ASSERT_IMPL("Debug Assert", cond, ": ", msg)
#else
# define GUI_DEBUG_ASSERT(cond) (void)(cond)
# define GUI_DEBUG_ASSERT_WITH_MESSAGE(cond, msg) (void)(cond)
#endif

#define GUI_ASSERT(cond)                         GUI_ASSERT_IMPL("Assert", cond, "", "")
#define GUI_ASSERT_WITH_MESSAGE(cond, msg)       GUI_ASSERT_IMPL("Assert", cond, ": ", msg)

#define GUI_UNREACHABLE(msg) do { fprintf(stderr, "Unreachable %s:%d: %s\n", __FILE__, __LINE__, (msg)); exit(1); } while(false)
#define GUI_TODO(msg)        do { fprintf(stderr, "Todo %s:%d: %s\n", __FILE__, __LINE__, (msg)); exit(1); } while(false)

#ifdef _MSC_VER
# define _CRT_SECURE_NO_WARNINGS 1
#endif
