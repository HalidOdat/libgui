cmake_minimum_required(VERSION 3.5)

set(This EmbedderBin)

# Workaround see: https://stackoverflow.com/questions/27168094/cmake-how-to-change-compiler-for-individual-target
if (WEB)
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)

  message(INFO "Compiling ${This} with native compiler in web mode")
endif()

add_executable(${This} main.cpp)
target_link_libraries(${This} -static-libstdc++)
