#include "ShaderProgram.h"
#include "Utils.hpp"

ShaderProgram::ShaderProgram(const char *vertexShaderFileName, const char *fragmentShaderFileName)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    char* fileString;
    Utils::readTextFromFile(vertexShaderFileName, fileString);

    glShaderSource(vertexShader, 1, (const char**)&fileString, nullptr);
    glCompileShader(vertexShader);

    Utils::readTextFromFile(fragmentShaderFileName, fileString);

    glShaderSource(fragmentShader, 1, (const char**)&fileString, nullptr);
    glCompileShader(fragmentShader);

    programHandle = glCreateProgram();

    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);

    glLinkProgram(programHandle);

    // clean up memory
    delete fileString;
}