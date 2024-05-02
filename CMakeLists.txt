cmake_minimum_required(VERSION 3.25)

project(cellular-automata LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

find_package(SFML COMPONENTS graphics REQUIRED)
find_package(tomlplusplus REQUIRED)

add_executable(
  cellular-automata
  src/simulation.cpp
  src/config.cpp
  src/main.cpp
)

target_link_libraries(cellular-automata PRIVATE sfml-graphics)
target_link_libraries(cellular-automata PRIVATE tomlplusplus)
