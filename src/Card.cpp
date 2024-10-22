#include "Card.h"
#include "Engine.h"

Card::Card(Engine* pEngine, ShaderProgram *pShaderProgram, ShaderProgram* pScreenSpaceShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, glm::vec2 hitBoxSize) : GameObject(pEngine, pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
{
    setScale(glm::vec3(0.63f, 0.885f, 0.005f));
    this->pHitBox = new HitBox(this, hitBoxSize);
    this->pScreenSpaceShaderProgram = pScreenSpaceShaderProgram;
    inHand = false;
    isTapped = false;
}

void Card::onLeftClick() {
    if (pEngine->getIsDoubleClick()) {
        toggleIsTapped();
    }
    else {
        isSelected = true;
    }
}

void Card::onLeftRelease() {
    isSelected = false;
    if (pEngine->getMousePosition().y > pEngine->getWindowSize().y - pEngine->getHandScreenThreshold()) {
        pEngine->sendToHand(this);
    }

    pEngine->drop(this);
}

void Card::onHover()
{
    // draw the hovered card in screen space big in a set location
    glm::vec3 tempPosition = position;
    glm::vec3 tempRotation = rotation;
    glm::vec3 tempScale = scale;
    bool tempInHand = inHand;
    inHand = true; // not actually in hand, just needs to use screen space shader
                    // aka bad code

    glm::vec3 cardPreviewSize = (glm::vec3(672.0f, 936.0f, 1.0f));

    // scale the preview for small screens
    while (cardPreviewSize.y > pEngine->getWindowSize().y) {
        cardPreviewSize *= 0.5f;
    }

    GLfloat padding = 100;

    // check which side of the screen the cursor is on, preview the card on the other side
    if (pEngine->getMousePosition().x > pEngine->getWindowSize().x / 2.0f) {
        setPosition(glm::vec3(padding, pEngine->getWindowSize().y / 2.0f - cardPreviewSize.y / 2.0f, 0.0f));
    }
    else {
        setPosition(glm::vec3(pEngine->getWindowSize().x - cardPreviewSize.x - padding, pEngine->getWindowSize().y / 2.0f - cardPreviewSize.y / 2.0f, 0.0f));
    }
    setRotation(glm::vec3(0.0f));
    setScale(cardPreviewSize);

    draw(pEngine->getCamera());

    inHand = tempInHand; // back to where it was
    setPosition(tempPosition);
    setRotation(tempRotation);
    setScale(tempScale);
}

void Card::update()
{
    if (isSelected) {
        if (!inHand) {
            float yOffset = 0.5f;
            glm::vec3 mouseRay = Utils::calculateCursorRay(pEngine->getWindowSize(), pEngine->getMousePosition(), pEngine->getCamera());
            glm::vec3 groundPoint = Utils::calculateRayIntersection(pEngine->getCamera()->getPosition(), mouseRay);
            glm::vec3 offset = glm::vec3(-scale.x / 2.0f, -scale.y / 2.0f, 0.2f);
            setPosition(groundPoint + offset);
        }
        else {
            sendToBoard();
            pEngine->removeFromHand(this);
        }
    }
}

void Card::select()
{
    isSelected = true;
}

bool Card::check2DPointCollision(glm::vec2 point)
{
    if (point.x < position.x + scale.x && point.x > position.x && 
        point.y > position.z - scale.z && point.y < position.z) {
            return true;
    }
    return false;
}

float Card::checkRayCollision(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
    if (!inHand) {
        glm::vec3 point = Utils::calculateRayIntersection(rayOrigin, rayDirection);
        // i only  care about the xz plane, since generally cards are close to it
        if (point.x < position.x + scale.x && point.x > position.x && 
            point.y < position.y + scale.y && point.y > position.y) {
                GLfloat distanceToPoint = glm::distance(point, pEngine->getCamera()->getPosition());
                return distanceToPoint - 2.0f * position.z; // subtract the distance from y such that cards higher up have prio
        }
        return -1;
    }
    else {
        glm::vec2 cursorPosition = pEngine->getMousePosition();
        if (cursorPosition.x < position.x + scale.x && cursorPosition.x > position.x &&
            cursorPosition.y > pEngine->getWindowSize().y - position.y - scale.y && cursorPosition.y < pEngine->getWindowSize().y - position.y) {
                return 0.0f;
            }
        return -1;
    }
}

bool Card::checkVerticalCollision(GameObject *other)
{
    if (inHand) {
        return false;
    }

    return GameObject::checkVerticalCollision(other);
}

void Card::draw(Camera *pCamera)
{
    if (inHand) {
        pScreenSpaceShaderProgram->useProgram();
        glm::mat4 projectionMatrix = pCamera->getOrthographicProjectionMatrix() * modelMatrix;
        glProgramUniformMatrix4fv(  pScreenSpaceShaderProgram->getProgramHandle(), 
                                    pScreenSpaceShaderProgram->getProjectionMatrixUniformLocation(), 
                                    1, GL_FALSE, &projectionMatrix[0][0]);
        glProgramUniform1i(pScreenSpaceShaderProgram->getProgramHandle(), pScreenSpaceShaderProgram->getTextureMapUniformLocation(), 0);

        glBindTexture(GL_TEXTURE_2D, textureHandle);
        glBindVertexArray(vaoHandle);
        glDrawElements(GL_TRIANGLES, numVAOPoints, GL_UNSIGNED_INT, 0);
    }
    else {
        if (isTapped) {
            glm::vec3 tempPos = position;
            setPosition(position + glm::vec3(0.0f, scale.x, 0.0f));
            GameObject::draw(pCamera);
            setPosition(tempPos);
        }
        else {
            GameObject::draw(pCamera);
        }
    }
}

void Card::toggleIsTapped() {
    if (isTapped) {
        untap();
    }
    else {
        tap();
    }
}

void Card::tap() {
    setRotation(glm::vec3(0.0f, 0.0f, glm::radians(-90.0f)));
    isTapped = true;
}

void Card::untap() {
    setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    isTapped = false;
}  

void Card::sendToHand()
{
    untap();
    setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    setScale(0.5f * glm::vec3(672, 936, 1.0f));
    inHand = true;
}

void Card::sendToBoard()
{
    setScale(glm::vec3(0.63f, 0.88f, 0.005f));
    inHand = false;
}
