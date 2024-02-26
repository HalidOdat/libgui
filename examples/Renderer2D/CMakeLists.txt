cmake_minimum_required(VERSION 3.5)

set(This ExampleRenderer2D)
add_executable(${This}
  main.cpp
)

target_link_libraries(${This} PRIVATE
  ${PROJECT_NAME}
  # Create static builds
  -static-libstdc++
)
