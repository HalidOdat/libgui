cmake_minimum_required(VERSION 3.5)

set(This tests)
add_executable(${This}
  main.cpp
)

# These tests can use the Catch2-provided main
target_link_libraries(${This} PRIVATE
  ${PROJECT_NAME}
  Catch2::Catch2WithMain
  # Create static builds
  -static-libstdc++
)

add_test(
  NAME    ${This}
  COMMAND ${This}
)
