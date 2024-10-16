#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>    // Include GLEW for OpenGL function loading

class ShaderProgram {
    public:
        ShaderProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);
        void useProgram();

        unsigned int getProgramHandle() { return programHandle; }
        unsigned int getMVPMatrixUniformLocation() { return mvpMatrixUniformLocation; }
        unsigned int getTextureMapUniformLocation() { return textureMapUniformLocation; }
    private:
        unsigned int programHandle;

        unsigned int mvpMatrixUniformLocation;
        unsigned int textureMapUniformLocation;
};

#endif