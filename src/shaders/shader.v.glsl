#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextureCoordinate;

uniform mat4 mvpMatrix;

out vec2 vertexTextureCoordinateOut;

void main()
{
    gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
    vertexTextureCoordinateOut = vertexTextureCoordinate;
}
