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
        Engine();
        int run();

        void addGameObject(GameObject* gameObject);
        void drop(GameObject* GameObject);

        void sendToHand(Card* card);
        void removeFromHand(Card* card);

        void checkIsDoubleClick();

        glm::vec2 getWindowSize() { return windowSize; }
        Camera* getCamera() { return pCamera; }
        glm::vec2 getMousePosition() { return cursorPosition; };
        GLfloat getHandScreenThreshold() { return handScreenThreshold; };
        
        void handleFramebufferSizeEven(int width, int height);
        void handleCursorPositionEvent(double x, double y );  
        void handleMouseButtonEvent(int button, int action, int mods );
        void handleScrollEvent(double xOffset, double yOffset);  

    private:
        GLFWwindow* pWindow;

        ShaderProgram* pShaderProgram;
        ShaderProgram* pScreenSpaceShaderProgram;

        Camera* pCamera;

        Deck* deck;
        std::vector<GameObject*> gameObjects;
        std::vector<Card*> hand;
        GLfloat handScreenThreshold;

        GameObject* table;
        
        Card* pHoveredCard;
        Card* pSelectedCard;

        glm::vec2 windowSize;

        static const int NUM_TEXTURES = 2;
        enum TEXTURE_ID {
            GRID = 0,
            BACK = 1
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
        GLfloat doubleClickThreshold = 0.25f;
        GLfloat lastClickTime = 0.0f;
        GLboolean isDoubleClick = false;

        int setupOpenGL();
        void SetupTextures();
        void SetupVAOs();

        void drawScene();

        Card* checkSelectedCard();
        Card* checkHoveredCard();
};

// Declare callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double x, double y );
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods );
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);