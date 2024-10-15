#include <GL/glew.h>    // Include GLEW for OpenGL function loading

class ShaderProgram {
    public:
        ShaderProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);
        void useProgram();

        unsigned int getProgramHandle() { return programHandle; }
    private:
        unsigned int programHandle;
};