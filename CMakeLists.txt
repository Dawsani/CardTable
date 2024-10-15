cmake_minimum_required(VERSION 3.5)
project(CardTable)

# the stbimage lib is here
include_directories("/usr/local/lib")

# Add source files
add_executable(CardTable 
                src/main.cpp
                src/Engine.cpp src/Engine.h
                src/Camera.cpp src/Camera.h
                src/Utils.hpp)

target_link_libraries(CardTable GL glfw GLEW stbimage)