#include <GL/glew.h>    // Include GLEW for OpenGL function loading
#include <GLFW/glfw3.h> // Include GLFW for window management
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include "Utils.hpp"

class Engine {
    public:
    void handleFramebufferSizeEven(int width, int height);
    void handleCursorPositionEvent(double x, double y );  
    void handleMouseButtonEvent(int button, int action, int mods );
    void handleScrollEvent(double xOffset, double yOffset);  

    private:

    static const int NUM_TEXTURES = 2;
    enum TEXTURE_ID {
        GRID = 0,
        SATYA = 1
    };
    unsigned int textureHandles[NUM_TEXTURES];

    static const int NUM_VAOS = 2;
    enum VAO_ID {
        TABLE = 0,
        CARD = 1
    };
    unsigned int vaoHandles[NUM_VAOS];

    unsigned int shaderProgram;

    // Camera
    glm::vec3 cameraPosition = glm::vec3(0, 3, -3);
    glm::vec3 cameraLookAtPoint = glm::vec3(0, 0, 0);   // looking at the origin
    glm::vec3 cameraUpVector = glm::vec3(0, 1, 0);    
    float cameraSpeed = 0.01f;  

    glm::vec2 cursorPosition;
    GLint rightMouseButtonState = GLFW_RELEASE;
    GLint leftMouseButtonState = GLFW_RELEASE;

    void SetupTextures();
    unsigned int CreateCard();
    unsigned int CreateTable();
    void SetupVAOs();
    void DrawCard(TEXTURE_ID cardTextureID, glm::vec3 cardPosition);
    int run();
    

};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);