cmake_minimum_required(VERSION 3.5)
project(CardTable)

# the stbimage lib is here
include_directories("/usr/local/lib")

# Add source files
add_executable(CardTable 
                src/main.cpp
                src/Engine.cpp src/Engine.h
                src/GameObject.cpp src/GameObject.h
                src/Card.cpp src/Card.h
                src/Deck.cpp src/Deck.h
                src/Camera.cpp src/Camera.h
                src/ShaderProgram.cpp src/ShaderProgram.h
                src/HitBox.cpp src/HitBox.h
                src/Utils.cpp src/Utils.h)

target_link_libraries(CardTable GL glfw GLEW stbimage curl)