#include "Engine.h"
#include <chrono>

Engine::Engine() {

}

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);  // Enable window resizing

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for Mac
#endif
    windowSize = glm::vec2(1280, 720);
    // Create a windowed mode window and its OpenGL context
    pWindow = glfwCreateWindow(windowSize.x, windowSize.y, "Card Table", NULL, NULL);
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

    //glfwSwapInterval(0);  // Disable VSync

    return 0;
}

void Engine::SetupTextures() {
    textureHandles[TEXTURE_ID::GRID] = Utils::LoadTexture("assets/textures/grid.png");
    textureHandles[TEXTURE_ID::BACK] = Utils::LoadTexture("assets/textures/card_back.jpg");
}

void Engine::SetupVAOs() {
    Utils::loadModel("assets/models/table.obj", vaoHandles[VAO_ID::TABLE], numVAOPoints[VAO_ID::TABLE]);
    Utils::loadModel("assets/models/card.obj", vaoHandles[VAO_ID::CARD], numVAOPoints[VAO_ID::CARD]);
    Utils::loadModel("assets/models/deck.obj", vaoHandles[VAO_ID::DECK], numVAOPoints[VAO_ID::DECK]);
}

void Engine::drawScene() {
    pShaderProgram->useProgram();

    for (GameObject* g : gameObjects) {
        g->draw(pCamera);
    }

}

/*
Card* Engine::checkSelectedCard() {
    float handScreenThreshold = windowSize.y - 100.0f;
    if (leftMouseButtonState == GLFW_PRESS && pSelectedCard == nullptr) {
        glm::vec3 cursorRay = Utils::calculateCursorRay(windowSize, cursorPosition, pCamera);
        glm::vec3 intersectionPoint = Utils::calculateRayIntersection(pCamera->getPosition(), cursorRay);

        if (cursorPosition.y > handScreenThreshold) {
            // check if they clicked a card
            for (int i = 0; i < hand.size(); i++) {
                Card* c = hand[i];
                glm::vec2 cardPosition = glm::vec2(c->getPosition().x, c->getPosition().y);
                glm::vec2 cardSize = 2.0f * glm::vec2(c->getScale().x, c->getScale().y);

                // only checking x and y because anything above it isnt within the threshold and anything below is outside the screen
                if (cursorPosition.x > cardPosition.x && cursorPosition.x < cardPosition.x + cardSize.x) {
                    c->sendToBoard();
                    // remove from hand
                    for (int i = 0; i < hand.size(); i++) {
                        Card* cc = hand[i];
                        if (cc == c) {
                            hand.erase(hand.begin() + i);
                            break;
                        }
                    }
                    // add to board
                    cards.push_back(c);
                    c->setScale(glm::vec3(0.63f, 0.88f, 1.0f));
                    c->setRotation(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
                    return c;
                }
            }
        }
        else {
            if (intersectionPoint.x < deck->getPosition().x + 0.63f && intersectionPoint.x > deck->getPosition().x &&
                intersectionPoint.z < deck->getPosition().z + 0.88f && intersectionPoint.z > deck->getPosition().z) {
                    //Card* drawnCard = deck->drawCard();
                    //cards.push_back(drawnCard);
                    //return drawnCard;
                }
            Card* hoveredCard = Utils::findHoveredCard(windowSize, cursorPosition, pCamera, cards);
            
            if (isDoubleClick) {
                hoveredCard->toggleIsTapped();
                return nullptr;
            }

            return hoveredCard;
        }
    }
    else if (leftMouseButtonState == GLFW_RELEASE && pSelectedCard != nullptr) {
        // put the card in the hand if the mouse was near the bottom of the screen
        
        else {
            // Make sure the card lands on top of the cards below it
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
        }
        return nullptr;
    }
    return pSelectedCard;
}
*/

int Engine::run() {

    // Clear the screen to a color (e.g., black)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setupOpenGL();
    SetupTextures();
    SetupVAOs();

    std::cout << "Setting up shaders...";
    pShaderProgram = new ShaderProgram("shaders/shader.v.glsl", "shaders/shader.f.glsl");
    pScreenSpaceShaderProgram = new ShaderProgram("shaders/screenSpaceShader.v.glsl", "shaders/screenSpaceShader.f.glsl");
    std::cout << "Done" << std::endl;

    pCamera = new Camera(glm::vec3(0.0f, 3.0f, 2.0f), 45.0f, windowSize);

    // Main loop
    glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

    table = new GameObject(this, pShaderProgram, vaoHandles[VAO_ID::TABLE], numVAOPoints[VAO_ID::TABLE], textureHandles[TEXTURE_ID::GRID]);
    gameObjects.push_back(table);
    
    std::cout << "Reading in deck" << std::endl;
    std::deque<Card*> deckCards = Utils::readCardsFromFile(this, "assets/deck_lists/my_deck.txt", pShaderProgram, pScreenSpaceShaderProgram, vaoHandles[VAO_ID::CARD], numVAOPoints[VAO_ID::CARD], glm::vec2(0.63f, 0.88f));
    deck = new Deck(this, pShaderProgram, vaoHandles[VAO_ID::DECK], numVAOPoints[VAO_ID::DECK], textureHandles[TEXTURE_ID::BACK], deckCards);
    std::cout << "Deck loaded succesfully." << std::endl;
    gameObjects.push_back(deck);

    handScreenThreshold = 400;

    pSelectedCard = nullptr;
    auto previousTime = std::chrono::high_resolution_clock::now();
    
    while (!glfwWindowShouldClose(pWindow)) {
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Calculate the time difference (delta time) between the current and previous frame
        std::chrono::duration<float> deltaTime = currentTime - previousTime;

        // Update the previous time to the current time
        previousTime = currentTime;

        // Convert the delta time to seconds for easy use
        float deltaSeconds = deltaTime.count();
        float fps = 1.0f / deltaSeconds;

        // Print delta time for demonstration purposes
        //std::cout << "Frame Time: " << deltaSeconds << " seconds" << std::endl;
        //std::cout << "FPS: " << fps << std::endl;
        
        // Clear the screen to a color (e.g., black)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
        pHoveredCard = Utils::findHoveredCard(windowSize, cursorPosition, pCamera, cards);

        pSelectedCard = checkSelectedCard();

        if (pSelectedCard != nullptr) {
            glm::vec3 cursorRay = Utils::calculateCursorRay(windowSize, cursorPosition, pCamera);
            glm::vec3 intersectionPoint = Utils::calculateRayIntersection(pCamera->getPosition(), cursorRay);
            pSelectedCard->setPosition(intersectionPoint + glm::vec3(-0.63f / 2.0f, 0.5f, 0.88f / 2.0f));
        }
        */
        GameObject* hoveredObject = Utils::findHoveredGameObject(windowSize, cursorPosition, pCamera, gameObjects);
        if (hoveredObject) {
            hoveredObject->onHover();
        }

        for (GameObject* g : gameObjects) {
            g->update();
        }

        for (int i = 0; i < hand.size(); i++) {
            Card* c = hand[i];
            c->setPosition(glm::vec3(i * c->getScale().x, 0.0f, 0.0f));
        }

        drawScene();

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

void Engine::addGameObject(GameObject *gameObject)
{
    gameObjects.push_back(gameObject);
}

void Engine::drop(GameObject *gameObject)
{
    // find the highest point on every object
    GLfloat highestPoint = 0.0f;
    for (GameObject* g : gameObjects) {
        if (g == gameObject) {
            continue;
        }

        if (gameObject->checkVerticalCollision(g)) {
            GLfloat topPoint = g->getPosition().y + g->getScale().y;
            if (topPoint > highestPoint) {
                highestPoint = topPoint;
            }
        }
    }

    gameObject->setPosition(glm::vec3(gameObject->getPosition().x, highestPoint + 0.005f, gameObject->getPosition().z));
}

void Engine::sendToHand(Card *card)
{
    hand.push_back(card);
    card->sendToHand();
}

void Engine::removeFromHand(Card *card)
{
    for (int i = 0; i < hand.size(); i++) {
        if (hand[i] == card) {
            hand.erase(hand.begin() + i);
            card->sendToBoard();
            return;
        }
    }
}

void Engine::handleFramebufferSizeEven(int width, int height) {
        glViewport(0, 0, width, height);
        windowSize = glm::vec2(width, height);
        pCamera->updateProjectionMatrices(windowSize);
}

void Engine::handleCursorPositionEvent(double x, double y) {
    glm::vec2 currentCursorPosition = glm::vec2(x, y);

    if (rightMouseButtonState == GLFW_PRESS) {

        // disable the curser until they release mouse button
        glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glm::vec2 cursorMovement = cursorPosition - currentCursorPosition;
        
        pCamera->Pan(cursorMovement);
    }

    cursorPosition = currentCursorPosition;
}

void Engine::checkIsDoubleClick() {
    GLfloat currentTime = glfwGetTime();
    if (currentTime - lastClickTime < doubleClickThreshold) {
        isDoubleClick = true;
        std::cout << "Double click!" << std::endl;
    }
    else {
        isDoubleClick = false;
    }
    lastClickTime = currentTime;
}

void Engine::handleMouseButtonEvent(int button, int action, int mod) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightMouseButtonState = action;
        if (rightMouseButtonState == GLFW_RELEASE) {
            glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            GameObject* hoveredGameObject = Utils::findHoveredGameObject(windowSize, cursorPosition, pCamera, gameObjects);
            if (!hoveredGameObject) {
                return;
            }
            checkIsDoubleClick();
            hoveredGameObject->onLeftClick();
        }
        else if (action == GLFW_RELEASE) {
            for (GameObject* g : gameObjects) {
                if (g->getIsSelected()) {
                    g->onLeftRelease();
                }
            }
        }
    }
}

void Engine::handleScrollEvent(double xOffset, double yOffset) {
    pCamera->moveForward(-yOffset);
}
