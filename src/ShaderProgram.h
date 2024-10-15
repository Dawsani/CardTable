#include <GL/glew.h>    // Include GLEW for OpenGL function loading

class ShaderProgram {
    public:
        ShaderProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);
        unsigned int programHandle;
    private:
};