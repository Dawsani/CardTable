#version 330 core

uniform sampler2D textureMap;
in vec2 vertexTextureCoordinateOut;

out vec4 fragColorOut;

void main()
{
    vec4 textureRGBA =  texture(textureMap, vertexTextureCoordinateOut);

    fragColorOut = textureRGBA;
}
