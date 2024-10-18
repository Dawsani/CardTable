#include "ShaderProgram.h"
#include "Utils.h"

ShaderProgram::ShaderProgram(const char *vertexShaderFileName, const char *fragmentShaderFileName)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    char* fileString;
    Utils::readTextFromFile(vertexShaderFileName, fileString);

    glShaderSource(vertexShader, 1, (const char**)&fileString, nullptr);
    glCompileShader(vertexShader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        // If the shader failed to compile, get the error message
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader Compilation Failed\n" << infoLog << std::endl;
    }

    Utils::readTextFromFile(fragmentShaderFileName, fileString);

    glShaderSource(fragmentShader, 1, (const char**)&fileString, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        // If the shader failed to compile, get the error message
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader Compilation Failed\n" << infoLog << std::endl;
    }

    programHandle = glCreateProgram();

    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);

    glLinkProgram(programHandle);

    // Get program uniforms
    mvpMatrixUniformLocation = glGetUniformLocation(programHandle, "mvpMatrix");
    textureMapUniformLocation = glGetUniformLocation(programHandle, "textureMap");
    projectionMatrixUniformLocation = glGetUniformLocation(programHandle, "projectionMatrix");
}

void ShaderProgram::useProgram(){
    glUseProgram(programHandle);
}
