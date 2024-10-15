#include <GL/glew.h>    // Include GLEW for OpenGL function loading
#include <GLFW/glfw3.h> // Include GLFW for window management
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include "Utils.hpp"
#include "Camera.h"
#include "ShaderProgram.h"

class Engine {
    public:
        int run();
        
        void handleFramebufferSizeEven(int width, int height);
        void handleCursorPositionEvent(double x, double y );  
        void handleMouseButtonEvent(int button, int action, int mods );
        void handleScrollEvent(double xOffset, double yOffset);  

    private:

        GLFWwindow* pWindow;

        ShaderProgram* pShaderProgram;

        Camera* pCamera;

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

        glm::vec2 cursorPosition;
        GLint rightMouseButtonState = GLFW_RELEASE;
        GLint leftMouseButtonState = GLFW_RELEASE;

        void SetupTextures();
        unsigned int CreateCard();
        unsigned int CreateTable();
        void SetupVAOs();
        void DrawCard(TEXTURE_ID cardTextureID, glm::vec3 cardPosition);    
};

// Declare callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double x, double y );
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods );
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);