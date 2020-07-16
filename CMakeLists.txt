cmake_minimum_required(VERSION 2.6)
project(fireworks)

add_executable(fireworks main.cpp)

link_directories(/usr/local/lib)
target_link_libraries(fireworks sfml-graphics sfml-window sfml-system)
