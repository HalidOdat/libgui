cmake_minimum_required(VERSION 3.5)

set(This Embedder)
add_library(${This} STATIC
  include/Asset.hpp
  include/RootAsset.hpp

  src/Asset.cpp
  src/RootAsset.cpp
)
target_include_directories(${This} PUBLIC include)


set(This EmbedderBin)
add_executable(${This}
  bin/main.cpp
)

add_custom_command(
  OUTPUT  ${CMAKE_BINARY_DIR}/assets.hpp ${CMAKE_BINARY_DIR}/assets.cpp
  COMMAND $<TARGET_FILE:EmbedderBin>
  ARGS    ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
  # DEPENDS ${PROJECT_SOURCE_DIR}/assets
  COMMENT "Generating file using custom command"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  VERBATIM
)

add_custom_target(custom_target
  ALL DEPENDS ${CMAKE_BINARY_DIR}/assets.hpp ${CMAKE_BINARY_DIR}/assets.cpp
  SOURCES ${CMAKE_BINARY_DIR}/assets.hpp ${CMAKE_BINARY_DIR}/assets.cpp
)

add_library(EmbeddedAssets STATIC
  ${CMAKE_BINARY_DIR}/assets.hpp
  ${CMAKE_BINARY_DIR}/assets.cpp
)

target_include_directories(EmbeddedAssets PUBLIC ${CMAKE_BINARY_DIR}/)

target_link_libraries(EmbeddedAssets PUBLIC Embedder)
