#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextureCoordinate;

uniform mat4 projectionMatrix;

out vec2 vertexTextureCoordinateOut;

void main()
{
    gl_Position = projectionMatrix * vec4(vertexPosition, 1.0);
    vertexTextureCoordinateOut = vertexTextureCoordinate;
}
