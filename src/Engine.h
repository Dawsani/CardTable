#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <vector>
#include "Utils.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "GameObject.h"
#include "Card.h"
#include "Deck.h"

class Engine {
    public:
        int run();
        
        void handleFramebufferSizeEven(int width, int height);
        void handleCursorPositionEvent(double x, double y );  
        void handleMouseButtonEvent(int button, int action, int mods );
        void handleScrollEvent(double xOffset, double yOffset);  

    private:

        glm::vec3 calculateCursorRay();
        glm::vec3 calculateRayIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection);

        GLFWwindow* pWindow;

        ShaderProgram* pShaderProgram;
        ShaderProgram* pScreenSpaceShaderProgram;

        Camera* pCamera;

        std::vector<Card*> cards;

        Card* pSelectedCard;
        glm::vec3 grabPoint;

        static const int NUM_TEXTURES = 3;
        enum TEXTURE_ID {
            GRID = 0,
            SATYA = 1,
            BACK = 2
        };
        unsigned int textureHandles[NUM_TEXTURES];

        static const int NUM_VAOS = 3;
        enum VAO_ID {
            TABLE = 0,
            CARD = 1,
            DECK = 2
        };
        unsigned int vaoHandles[NUM_VAOS];
        unsigned int numVAOPoints[NUM_VAOS];

        glm::vec2 cursorPosition;
        GLint rightMouseButtonState = GLFW_RELEASE;
        GLint leftMouseButtonState = GLFW_RELEASE;

        int setupOpenGL();
        void SetupTextures();
        void SetupVAOs();
};

// Declare callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double x, double y );
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods );
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);