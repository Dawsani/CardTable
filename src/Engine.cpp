#include "Engine.h"
#include <chrono>

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
    windowSize = glm::vec2(800, 600);
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

    // glfwSwapInterval(0);  // Disable VSync

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

void Engine::drawScene() {
    pShaderProgram->useProgram();

    table->draw(pCamera);
    // deck->draw(pCamera);

    for (int i = 0; i < cards.size(); i++) {
        GameObject* card = cards[i];
        card->draw(pCamera);
    }

    for (int i = 0; i < hand.size(); i++) {
        GameObject* card = hand[i];
        card->setPosition(glm::vec3(2 * 63.0f * i, 0.0f, 0.0f));
        card->setScale(2.0f * glm::vec3(63.0f, 88.0f, 1.0f));
        card->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        card->draw(pCamera);
    }

}

Card* Engine::checkSelectedCard() {
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
            return Utils::findHoveredCard(windowSize, cursorPosition, pCamera, cards);
        }
    }
    else if (leftMouseButtonState == GLFW_RELEASE && pSelectedCard != nullptr) {
        // put the card in the hand if the mouse was near the bottom of the screen
        if (cursorPosition.y > handScreenThreshold) {
            pSelectedCard->sendToHand();
            hand.push_back(pSelectedCard);
            // remove card from the list of played cards
            // first find the position of the card within the vector
            for (int i = 0; i < cards.size(); i++) {
                Card* c = cards.at(i);
                if (c == pSelectedCard) {
                    cards.erase(cards.begin() + i);
                    break;
                }
            }
        }
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

int Engine::run() {

    setupOpenGL();
    SetupTextures();
    SetupVAOs();

    pShaderProgram = new ShaderProgram("shaders/shader.v.glsl", "shaders/shader.f.glsl");
    pScreenSpaceShaderProgram = new ShaderProgram("shaders/screenSpaceShader.v.glsl", "shaders/screenSpaceShader.f.glsl");
    
    pCamera = new Camera(glm::vec3(0.0f, 3.0f, 2.0f));

    // Main loop
    glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

    table = new GameObject(pShaderProgram, vaoHandles[VAO_ID::TABLE], numVAOPoints[VAO_ID::TABLE], textureHandles[TEXTURE_ID::GRID]);

    std::vector<Card*> deckCards = Utils::readCardsFromFile("assets/deck_lists/my_deck.txt");
    Deck* deck = new Deck(pShaderProgram, vaoHandles[VAO_ID::DECK], numVAOPoints[VAO_ID::DECK], textureHandles[TEXTURE_ID::BACK], deckCards);

    // make some cards for now
    for (int i = 0; i < 16; i++) {
        Card* card = new Card(  pShaderProgram, 
                                pScreenSpaceShaderProgram,
                                            vaoHandles[VAO_ID::CARD],
                                            numVAOPoints[VAO_ID::CARD],
                                            textureHandles[TEXTURE_ID::SATYA],
                                            glm::vec2(0.63f, 0.88f));
        card->setPosition(glm::vec3(i, 0.02f, 0.0f));
        card->setRotation(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
        card->setScale(glm::vec3(0.63f, 0.88f, 1.0f));

        cards.push_back(card);
    }

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

        pHoveredCard = Utils::findHoveredCard(windowSize, cursorPosition, pCamera, cards);

        // draw the hovered card in screen space big in a set location
        if (pHoveredCard != nullptr) {
            glm::vec3 boardPosition = pHoveredCard->getPosition();
            pHoveredCard->sendToHand();

            glm::vec3 cardPreviewSize = (4.0f * glm::vec3(63.0f, 88.0f, 1.0f));
            GLfloat padding = 100;

            // check which side of the screen the cursor is on, preview the card on the other side
            if (cursorPosition.x > windowSize.x / 2.0f) {
                pHoveredCard->setPosition(glm::vec3(padding, windowSize.y / 2.0f - cardPreviewSize.y / 2.0f, 0.0f));
            }
            else {
                pHoveredCard->setPosition(glm::vec3(windowSize.x - cardPreviewSize.x - padding, windowSize.y / 2.0f - cardPreviewSize.y / 2.0f, 0.0f));
            }

            pHoveredCard->setScale(cardPreviewSize);
            pHoveredCard->setRotation(glm::vec3(0.0f));

            pHoveredCard->draw(pCamera);

            pHoveredCard->sendToBoard();
            pHoveredCard->setPosition(boardPosition);
            pHoveredCard->setScale(glm::vec3(0.63f, 0.88f, 1.0f));
            pHoveredCard->setRotation(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
        }

        pSelectedCard = checkSelectedCard();

        if (pSelectedCard != nullptr) {
            glm::vec3 cursorRay = Utils::calculateCursorRay(windowSize, cursorPosition, pCamera);
            glm::vec3 intersectionPoint = Utils::calculateRayIntersection(pCamera->getPosition(), cursorRay);
            pSelectedCard->setPosition(intersectionPoint + glm::vec3(-0.63f / 2.0f, 0.2f, 0.88f / 2.0f));
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
