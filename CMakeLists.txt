cmake_minimum_required(VERSION 3.5)

set(This Gui)

project(${This}
  VERSION 0.1.0
  LANGUAGES CXX C
  DESCRIPTION "A GUI library written in C++"
  # HOMEPAGE_URL "https://github.com/HalidOdat/"
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
  set(WEB 1)
endif()

# Check if we are executing cmake in the root directory.
set(GUI_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(GUI_MAIN_PROJECT ON)
endif()

set(GUI_BUILD_TESTS ${GUI_MAIN_PROJECT})

add_subdirectory(external)

set(ASSETS_DIRECTOY "${PROJECT_SOURCE_DIR}/assets")
message(STATUS "${This}: Assets directory path: ${ASSETS_DIRECTOY}")

add_library(${This} STATIC
  src/Core/Type.hpp
  src/Core/Macro.hpp
  src/Core/Log.hpp
  src/Core/Log.cpp
  src/Core/Color.hpp
  src/Core/OpenGL.hpp
  src/Core/Base.hpp
  
  src/Utils/File.hpp
  src/Utils/File.cpp
  src/Utils/String.hpp
  src/Utils/String.cpp

  src/Events/Event.hpp
  src/Events/Event.cpp
  src/Events/KeyEvent.hpp
  src/Events/MouseEvent.hpp
  src/Events/WindowEvent.hpp
  src/Events/FileDropEvent.hpp

  src/Renderer/Texture.hpp
  src/Renderer/Texture.cpp
  src/Renderer/Shader.hpp
  src/Renderer/Shader.cpp
  src/Renderer/VertexBuffer.hpp
  src/Renderer/VertexBuffer.cpp
  src/Renderer/IndexBuffer.hpp
  src/Renderer/IndexBuffer.cpp
  src/Renderer/VertexArray.hpp
  src/Renderer/VertexArray.cpp
  src/Renderer/FrameBuffer.hpp
  src/Renderer/FrameBuffer.cpp
  src/Renderer/UniformBuffer.hpp
  src/Renderer/UniformBuffer.cpp
  src/Renderer/Camera.hpp
  src/Renderer/Camera.cpp
  src/Renderer/CameraController.hpp
  src/Renderer/CameraController.cpp
  src/Renderer/Renderer2D.hpp
  src/Renderer/Renderer2D.cpp

  src/Window.hpp
  src/Window.cpp
)

target_include_directories(${This} PUBLIC src)

if (MSVC)
  target_compile_options(${This}
    PRIVATE
      /W4
  )
else()
  target_compile_options(${This}
    PRIVATE
      -Wall -Wextra -pedantic               # Enable very useful warnings
      -fno-exceptions                       # Disable exceptions
      -fno-rtti                             # Disable runtime type information (dynamic_cast)
      # $<$<AND:$<CONFIG:Debug>,$<WEB:>>:-Og>
      $<$<CONFIG:Release>:-Os>
      $<$<CONFIG:MinSizeRel>:-Oz>
  )
  target_link_options(${This}
    PRIVATE
      -fno-exceptions
      -fno-rtti
      # $<$<CONFIG:Debug>:-Og>
      $<$<CONFIG:Release>:-Os>
      $<$<CONFIG:MinSizeRel>:-Oz>
  )
  if (GUI_SANITIZER)
    target_compile_options(${This}
      PRIVATE
        -fsanitize=address
        -fsanitize=undefined
        -fsanitize=null
    )
    target_link_options(${This}
      PRIVATE
        -fsanitize=address
        -fsanitize=undefined
        -fsanitize=null
    )
  endif()
endif()

if (DEFINED WEB)
  message(STATUS "${This}: Building for web")
  if (NOT UNIX)
    message(FATAL_ERROR "${This}: Building for web on windows could cause some errors. Please build in WSL")
  endif()

  target_link_options(${This}
    PUBLIC
      -sFULL_ES3=1
      -sUSE_WEBGL2=1
      -sMIN_WEBGL_VERSION=2
      -sMAX_WEBGL_VERSION=2
      -sALLOW_MEMORY_GROWTH=1
      -sDISABLE_EXCEPTION_CATCHING=1
      # $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:--closure=1>
      $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:-sLZ4>
      $<$<CONFIG:MinSizeRel>:-flto=full>
      --preload-file "${ASSETS_DIRECTOY}@assets"
  )
else()
  message(STATUS "${This}: Building for native")
endif()

target_link_libraries(${This} PUBLIC
  glfw::glfw
  glm::glm
  stb::stb
  yaml-cpp::yaml-cpp
)

if (NOT DEFINED WEB)
  target_link_libraries(${This} PUBLIC
    glad::glad
  )
endif()

target_precompile_headers(${This}
  PUBLIC
    # Common include
    <Core/Base.hpp>

  PRIVATE
    # Standard library
    <cstdio>
    <cstdlib>
    <cstdint>
    <cstddef>
    <string>
    <string_view>
    <vector>
    <array>
    <memory>
    <optional>
    <unordered_map>
    <type_traits>
)

target_compile_definitions(${This}
  PUBLIC
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:GUI_DEBUG_MODE=1>
    $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:GATE_RELEASE_MODE=1>
)

if(GUI_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

if (GUI_MAIN_PROJECT)
  add_subdirectory(examples)
endif()