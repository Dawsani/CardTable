#include "Engine.h"

int Engine::setupOpenGL() {
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
    pWindow = glfwCreateWindow(800, 600, "Card Table", NULL, NULL);
    if (!pWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(pWindow);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set the viewport and callback for resizing the window
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    glViewport(0, 0, width, height);

    
    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(pWindow, cursor_position_callback);
    glfwSetMouseButtonCallback(pWindow, mouse_button_callback);
    glfwSetScrollCallback(pWindow, scroll_callback);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    return 0;
}

void Engine::SetupTextures() {
    textureHandles[TEXTURE_ID::GRID] = Utils::LoadTexture("assets/textures/test.png");
    textureHandles[TEXTURE_ID::SATYA] = Utils::LoadTexture("assets/textures/m3c-3-satya-aetherflux-genius.jpg");
    textureHandles[TEXTURE_ID::BACK] = Utils::LoadTexture("assets/textures/playing_cards/back.png");
}

void Engine::SetupVAOs() {
    Utils::loadModel("assets/models/table.obj", vaoHandles[VAO_ID::TABLE], numVAOPoints[VAO_ID::TABLE]);
    Utils::loadModel("assets/models/card.obj", vaoHandles[VAO_ID::CARD], numVAOPoints[VAO_ID::CARD]);
    Utils::loadModel("assets/models/deck.obj", vaoHandles[VAO_ID::DECK], numVAOPoints[VAO_ID::DECK]);
}

int Engine::run() {

    setupOpenGL();
    SetupTextures();
    SetupVAOs();

    pShaderProgram = new ShaderProgram("src/shaders/shader.v.glsl", "src/shaders/shader.f.glsl");
    pCamera = new Camera(glm::vec3(0.0f, 3.0f, 2.0f));

    // Main loop
    glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

    GameObject* table = new GameObject(pShaderProgram, vaoHandles[VAO_ID::TABLE], numVAOPoints[VAO_ID::TABLE], textureHandles[TEXTURE_ID::GRID]);

    std::vector<Card*> deckCards = Utils::readCardsFromFile("assets/deck_lists/my_deck.txt");
    Deck* deck = new Deck(pShaderProgram, vaoHandles[VAO_ID::DECK], numVAOPoints[VAO_ID::DECK], textureHandles[TEXTURE_ID::BACK], deckCards);

    for (int i = 0; i < 16; i++) {
        Card* card = new Card(  pShaderProgram, 
                                            vaoHandles[VAO_ID::CARD],
                                            numVAOPoints[VAO_ID::CARD],
                                            textureHandles[TEXTURE_ID::SATYA],
                                            glm::vec2(0.63f, 0.88f));
        card->setPosition(glm::vec3(i, 0.02f, 0.0f));
        card->setRotation(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));

        cards.push_back(card);
    }
    pSelectedCard = nullptr;
    
    while (!glfwWindowShouldClose(pWindow)) {
        // Clear the screen to a color (e.g., black)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pShaderProgram->useProgram();

        table->draw(pCamera);
        deck->draw(pCamera);

        glm::vec3 cursorRay = calculateCursorRay();
        glm::vec3 intersectionPoint = calculateRayIntersection(pCamera->getPosition(), cursorRay);

        if (leftMouseButtonState == GLFW_PRESS && pSelectedCard == nullptr) {
            std::vector<Card*> cardsUnderCursor;
            for (int i = 0; i < cards.size(); i++) {
                Card* card = cards[i];
                // check to see if the click selected a card
                glm::vec2 groundIntersection = glm::vec2(intersectionPoint.x, intersectionPoint.z);
                if (groundIntersection.x < card->getPosition().x + card->getHitBox()->getSize().x && groundIntersection.x > card->getPosition().x && 
                    intersectionPoint.z > card->getPosition().z - card->getHitBox()->getSize().y && intersectionPoint.z < card->getPosition().z) {
                        cardsUnderCursor.push_back(card);
                }
            }
            Card* highestCard = Utils::findHighestCard(cardsUnderCursor);

            if (highestCard != nullptr) {
                pSelectedCard = highestCard;
                grabPoint = glm::vec3(pSelectedCard->getPosition().x - intersectionPoint.x, 0.0f, pSelectedCard->getPosition().z - intersectionPoint.z);
            }
        }
        else if (leftMouseButtonState == GLFW_RELEASE && pSelectedCard != nullptr) {

            std::vector<Card*> cardsUnderSelectedCard;
            for (int i = 0; i < cards.size(); i++) {
                Card* card = cards[i];
                if (pSelectedCard == card) {
                    continue;
                }
                // check quad collision!
                if (pSelectedCard->getHitBox()->CheckCollision(card->getHitBox())) {
                    cardsUnderSelectedCard.push_back(card);
                }
            }

            Card* highestCardBelowSelectedCard = Utils::findHighestCard(cardsUnderSelectedCard);

            if (highestCardBelowSelectedCard != nullptr) {
                float highestYPosition = highestCardBelowSelectedCard->getPosition().y;
            
                pSelectedCard->setPosition(glm::vec3(pSelectedCard->getPosition().x, highestYPosition + 0.02f, pSelectedCard->getPosition().z));
            }
            else {
                pSelectedCard->setPosition(glm::vec3(pSelectedCard->getPosition().x, 0.02f, pSelectedCard->getPosition().z));
            }
            pSelectedCard = nullptr;
        }

        if (pSelectedCard != nullptr) {
            pSelectedCard->setPosition(intersectionPoint + glm::vec3(0.0f, 0.2f, 0.0f) + grabPoint);
        }

        for (int i = 0; i < cards.size(); i++) {
            GameObject* card = cards[i];
            card->draw(pCamera);
        }

        // Swap front and back buffers
        glfwSwapBuffers(pWindow);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up and exit
    glfwDestroyWindow(pWindow);
    glfwTerminate();
    return 0;
}

void Engine::handleFramebufferSizeEven(int width, int height) {
        glViewport(0, 0, width, height);
}

void Engine::handleCursorPositionEvent(double x, double y) {
    glm::vec2 currentCursorPosition = glm::vec2(x, y);

    if (rightMouseButtonState == GLFW_PRESS) {

        // disable the curser until they release mouse button
        glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glm::vec2 cursorMovement = cursorPosition - currentCursorPosition;
        
        pCamera->Pan(cursorMovement);
    }
    if (leftMouseButtonState == GLFW_PRESS) {
        // Check if the mouse clicked a card, skipping this for now and just grabbing the only card

    }

    cursorPosition = currentCursorPosition;
}

void Engine::handleMouseButtonEvent(int button, int action, int mod) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightMouseButtonState = action;
        if (rightMouseButtonState == GLFW_RELEASE) {
            glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        leftMouseButtonState = action;
    }
}

void Engine::handleScrollEvent(double xOffset, double yOffset) {
    pCamera->moveForward(-yOffset);
}

glm::vec3 Engine::calculateCursorRay()
{
    // get mouse coordinates in normalized device coords, but really clip space
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    glm::vec2 cursorPositionNDC = glm::vec2(2 * cursorPosition.x / width - 1, 1 - 2 * cursorPosition.y / height);
    glm::vec4 cursorPositionClipSpace = glm::vec4(cursorPositionNDC.x, cursorPositionNDC.y, -1.0f, 1.0f);
    glm::mat4 inverseProjectionMatrix = glm::inverse(pCamera->getProjectionMatrix());
    glm::vec4 cursorPositionViewSpace = inverseProjectionMatrix * cursorPositionClipSpace;
    glm::vec4 cursorRayViewSpace = glm::vec4(cursorPositionViewSpace.x, cursorPositionViewSpace.y, -1.0f, 0.0f);

    glm::mat4 inverseViewMatrix = glm::inverse(pCamera->getViewMatrix());
    glm::vec3 cursorRay = glm::normalize(inverseViewMatrix * cursorRayViewSpace);

    // flip some directions for some reasson
    return cursorRay;
}

glm::vec3 Engine::calculateRayIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
    rayDirection = glm::normalize(rayDirection);
    float t = - rayOrigin.y / rayDirection.y;
    glm::vec3 intersectionPoint = rayOrigin + rayDirection * t;

    // TODO: FIGURE OUT WHY I NEED TO MULTIPLY BY 2 RN.
    return intersectionPoint;
}
