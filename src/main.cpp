#include <GL/glew.h>    // Include GLEW for OpenGL function loading
#include <GLFW/glfw3.h> // Include GLFW for window management
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include "Utils.hpp"

const int NUM_TEXTURES = 2;
enum TEXTURE_ID {
    GRID = 0,
    SATYA = 1
};
unsigned int textureHandles[NUM_TEXTURES];

const int NUM_VAOS = 2;
enum VAO_ID {
    TABLE = 0,
    CARD = 1
};
unsigned int vaoHandles[NUM_TEXTURES];

unsigned int shaderProgram;

void SetupTextures() {
    textureHandles[TEXTURE_ID::GRID] = Utils::LoadTexture("assets/textures/test.png");
    std::cout << textureHandles[TEXTURE_ID::GRID] << std::endl;
    textureHandles[TEXTURE_ID::SATYA] = Utils::LoadTexture("assets/textures/m3c-3-satya-aetherflux-genius.jpg");
    std::cout << textureHandles[TEXTURE_ID::SATYA] << std::endl;
}

glm::vec3 cameraPosition = glm::vec3(0, 3, -3);
glm::vec3 cameraLookAtPoint = glm::vec3(0, 0, 0);   // looking at the origin
glm::vec3 cameraUpVector = glm::vec3(0, 1, 0);    
float cameraSpeed = 0.01f;  

glm::vec2 cursorPosition;
GLint rightMouseButtonState = GLFW_RELEASE;
GLint leftMouseButtonState = GLFW_RELEASE;


// Window resize callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double x, double y ) {
    glm::vec2 currentCursorPosition = glm::vec2(x, y);

    if (rightMouseButtonState == GLFW_PRESS) {

        // disable the curser until they release mouse button
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glm::vec2 cursorMovement = cursorPosition - currentCursorPosition;
        
        glm::vec3 cameraTranslation = -cameraSpeed * glm::vec3(cursorMovement.x, 0, cursorMovement.y);
        cameraPosition += cameraTranslation;
        cameraLookAtPoint += cameraTranslation;
    }
    if (leftMouseButtonState == GLFW_PRESS) {
        // Check if the mouse clicked a card, skipping this for now and just grabbing the only card

    }

    cursorPosition = currentCursorPosition;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods ) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightMouseButtonState = action;
        if (rightMouseButtonState == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        leftMouseButtonState = action;
    }
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    cameraPosition += glm::vec3(0.0f, yOffset, -yOffset);
    if (cameraPosition.y < 1.0f) {
        cameraPosition.y = 1.0f;
        cameraPosition.z = -1.0f;
    }
    else if (cameraPosition.y > 20.0f) {
        cameraPosition.y = 20.0f;
        cameraPosition.z = -20.0f;
    }
}

unsigned int CreateCard() {
    // Vertex data for the table
    float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.63f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,  0.88f, 0.0f, 0.0f, 1.0f,
        0.63f,  0.88f, 0.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 3
    };

    // Generate vertex array and buffers
    unsigned int VAO, VBO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);   // vertex postion
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // texture coordinate
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    return VAO;
}

unsigned int CreateTable() {
    // Vertex data for the table
    float vertices[] = {
        -10.0f, 0.0f, -10.0f, 0.0f, 0.0f,
        10.0f, 0.0f, -10.0f, 20.0f, 0.0f,
        -10.0f,  0.0f, 10.0f, 0.0f, 20.0f,
        10.0f,  0.0f, 10.0f, 20.0f, 20.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 3
    };

    // Generate vertex array and buffers
    unsigned int VAO, VBO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);   // vertex postion
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // texture coordinate
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    return VAO;
}

void SetupVAOs() {
    vaoHandles[VAO_ID::TABLE] = CreateTable();
    vaoHandles[VAO_ID::CARD] = CreateCard();
}

void DrawCard(TEXTURE_ID cardTextureID, glm::vec3 cardPosition) {

    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraLookAtPoint, cameraUpVector );
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, cardPosition + glm::vec3(0.0f, 0.02f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mvpMatrix =  projectionMatrix * viewMatrix * modelMatrix;
    unsigned int mvpMatrixUniformLocation = glGetUniformLocation(shaderProgram, "mvpMatrix");
    glProgramUniformMatrix4fv(shaderProgram, mvpMatrixUniformLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
    unsigned int textureMapUniformLocation = glGetUniformLocation(shaderProgram, "textureMap");
    glProgramUniform1i(shaderProgram, textureMapUniformLocation, 0); // Set the texture to texture 0 for now, while we only have one texture

    glBindTexture(GL_TEXTURE_2D, textureHandles[TEXTURE_ID::SATYA]);
    glBindVertexArray(vaoHandles[VAO_ID::CARD]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set OpenGL version to 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for Mac
#endif

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Card Table", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set the viewport and callback for resizing the window
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Compile and link shaders (load your own shader loading utility or use raw OpenGL shader compilation)
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexShaderFileName =      "src/shaders/shader.v.glsl";
    const char* fragmentShaderFileName =    "src/shaders/shader.f.glsl";

    char* fileString;
    if (Utils::readTextFromFile(vertexShaderFileName, fileString) == false) {
        return -1;
    }

    glShaderSource(vertexShader, 1, (const char**)&fileString, nullptr);
    glCompileShader(vertexShader);

    if (Utils::readTextFromFile(fragmentShaderFileName, fileString) == false) {
        std::cout << "Could not read shader file: " << fragmentShaderFileName << std::endl;
        return -1;
    }

    glShaderSource(fragmentShader, 1, (const char**)&fileString, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    // clean up memory
    delete fileString;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    SetupTextures();
    SetupVAOs();

    // Main loop
    glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
    
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen to a color (e.g., black)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        viewMatrix = glm::lookAt(cameraPosition, cameraLookAtPoint, cameraUpVector );
        projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        
        modelMatrix = glm::mat4(1.0f);
        glm::mat4 mvpMatrix =  projectionMatrix * viewMatrix * modelMatrix;
        unsigned int mvpMatrixUniformLocation = glGetUniformLocation(shaderProgram, "mvpMatrix");
        glProgramUniformMatrix4fv(shaderProgram, mvpMatrixUniformLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
        unsigned int textureMapUniformLocation = glGetUniformLocation(shaderProgram, "textureMap");
        glProgramUniform1i(shaderProgram, textureMapUniformLocation, 0); // Set the texture to texture 0 for now, while we only have one texture

        // glActiveTexture(GL_TEXTURE0); // Not sure if this does anything
        glBindTexture(GL_TEXTURE_2D, textureHandles[TEXTURE_ID::GRID]);
        glBindVertexArray(vaoHandles[VAO_ID::TABLE]);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

        DrawCard(TEXTURE_ID::SATYA, glm::vec3(0.0f, 0.0f, 0.0f));
        DrawCard(TEXTURE_ID::SATYA, glm::vec3(2.0f, 0.0f, 1.0f));

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
