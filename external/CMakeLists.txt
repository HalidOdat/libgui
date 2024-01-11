cmake_minimum_required(VERSION 3.5)

include(FetchContent)

cmake_policy(SET CMP0135 NEW)

# =============== glfw3 ==================
if (NOT DEFINED WEB)
  set(This glfw)
  message(STATUS "Fetching ${This} library...")
  FetchContent_Declare(${This}
    URL https://github.com/glfw/glfw/archive/refs/tags/3.3.9.zip
  )

  set(GLFW_BUILD_DOCS         OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS        OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES     OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_INSTALL      OFF CACHE BOOL "" FORCE)
  set(GLFW_DOCUMENT_INTERNALS OFF CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(${This})
else()
  add_library(glfw INTERFACE)
  target_link_options(glfw
    INTERFACE
      -sUSE_GLFW=3
  )
endif()

add_library(glfw::glfw ALIAS glfw)

# ================ glad =====================
if (NOT DEFINED WEB)
  set(This glad)

  FetchContent_Declare(${This}
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG c # Can be a tag, a commit hash, or a branch name (master)
  )
  FetchContent_GetProperties(${This})
  FetchContent_MakeAvailable(${This})

  add_library(${This} STATIC
    ${${This}_SOURCE_DIR}/src/glad.c
  )

  set_target_properties(${This} PROPERTIES
      FOLDER libraries
      ARCHIVE_OUTPUT_DIRECTORY ${${This}_BINARY_DIR}
      LIBRARY_OUTPUT_DIRECTORY ${${This}_BINARY_DIR}
      RUNTIME_OUTPUT_DIRECTORY ${${This}_BINARY_DIR}
  )

  target_include_directories(${This} PUBLIC
    ${${This}_SOURCE_DIR}/include
  )

  add_library(${This}::${This} ALIAS ${This})
endif()

# glm
set(This glm)
message(STATUS "Fetching ${This} library...")
FetchContent_Declare(${This}
  URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip
)
FetchContent_MakeAvailable(${This})

# =============== yaml-cpp ==================
set(This yaml-cpp)
message(STATUS "Fetching ${This} library...")
FetchContent_Declare(${This}
  URL https://github.com/jbeder/yaml-cpp/archive/refs/tags/0.8.0.zip
)

set(YAML_CPP_BUILD_TOOLS   OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(${This})

# ================ stb image ================
set(This stb)
message(STATUS "Fetching ${This} library...")
FetchContent_Declare(${This}
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master # Can be a tag, a commit hash, or a branch name (master)
)
FetchContent_GetProperties(${This})
FetchContent_MakeAvailable(${This})

add_library(${This} INTERFACE)
target_include_directories(${This} INTERFACE ${${This}_SOURCE_DIR})

add_library(${This}::${This} ALIAS ${This})

# ========== Testing =================

# If we are not testing return early
if (NOT GUI_BUILD_TESTS)
  return()
endif()

include(CTest)

set(This Catch2)
message(STATUS "Fetching ${This} library...")
find_package(${This} 3 QUIET)

if (NOT ${This}_FOUND)
  FetchContent_Declare(${This}
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.1.zip
  )

  FetchContent_MakeAvailable(${This})
endif()
