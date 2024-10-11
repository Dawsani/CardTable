cmake_minimum_required(VERSION 3.5)
project(CardTable)

# Add source files
add_executable(CardTable 
                src/main.cpp
                src/Camera.h src/Camera.cpp
                src/Utils.hpp)

target_link_libraries(CardTable GL glfw GLEW)