cmake_minimum_required(VERSION 3.25)

project(cellular-automata LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(
  cellular-automata
  src/main.cpp
)

target_link_libraries(cellular-automata PRIVATE sfml-graphics)
