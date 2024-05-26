cmake_minimum_required(VERSION 3.5)

set(This Calculator)
add_executable(${This}
  eval.hpp
  eval.cpp
  main.cpp
)

target_include_directories(${This} PRIVATE .)

target_link_libraries(${This} PRIVATE
  ${PROJECT_NAME}
  # Create static builds
  -static-libstdc++
)
