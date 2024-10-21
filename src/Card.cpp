#include "Card.h"
#include "Engine.h"

Card::Card(Engine* pEngine, ShaderProgram *pShaderProgram, ShaderProgram* pScreenSpaceShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, glm::vec2 hitBoxSize) : GameObject(pEngine, pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
{
    this->pHitBox = new HitBox(this, hitBoxSize);
    this->pScreenSpaceShaderProgram = pScreenSpaceShaderProgram;
    inHand = false;
}

void Card::onLeftClick() {
    isSelected = true;
}

void Card::onLeftRelease() {
    isSelected = false;
    /*
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
    */
}

void Card::onHover()
{
    // draw the hovered card in screen space big in a set location
    glm::vec3 tempPosition = position;
    glm::vec3 tempRotation = rotation;
    glm::vec3 tempScale = scale;
    inHand = true; // not actually in hand, just needs to use screen space shader
                    // aka bad code

    glm::vec3 cardPreviewSize = (8.0f * glm::vec3(63.0f, 88.0f, 1.0f));
    GLfloat padding = 100;

    // check which side of the screen the cursor is on, preview the card on the other side
    if (pEngine->getMousePosition().x > pEngine->getWindowSize().x / 2.0f) {
        setPosition(glm::vec3(padding, pEngine->getWindowSize().y / 2.0f - cardPreviewSize.y / 2.0f, 0.0f));
    }
    else {
        setPosition(glm::vec3(pEngine->getWindowSize().x - cardPreviewSize.x - padding, pEngine->getWindowSize().y / 2.0f - cardPreviewSize.y / 2.0f, 0.0f));
    }
    setScale(cardPreviewSize);
    setRotation(glm::vec3(0.0f));

    draw(pEngine->getCamera());

    inHand = false; // back to where it was
    setPosition(tempPosition);
    setScale(tempScale);
    setRotation(tempRotation);
}

void Card::update()
{
    if (isSelected) {
        float yOffset = 0.5f;
        glm::vec3 mouseRay = Utils::calculateCursorRay(pEngine->getWindowSize(), pEngine->getMousePosition(), pEngine->getCamera());
        glm::vec3 groundPoint = Utils::calculateRayIntersection(pEngine->getCamera()->getPosition(), mouseRay);
        glm::vec3 offset = glm::vec3(-scale.x / 2.0f, 0.05f, scale.z / 2.0f);
        setPosition(groundPoint + offset);
    }
}

void Card::select()
{
    isSelected = true;
}

bool Card::check2DPointCollision(glm::vec2 point)
{
    if (point.x < position.x + pHitBox->getSize().x && point.x > position.x && 
        point.y > position.z - pHitBox->getSize().y && point.y < point.y) {
            return true;
    }
    return false;
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
        GameObject::draw(pCamera);
    }
}

void Card::toggleIsTapped() {
    isTapped = !isTapped;
    if (isTapped) {
        setRotation(glm::vec3(rotation.x, glm::radians(90.0f), 0.0f));
    }
    else {
        setRotation(glm::vec3(rotation.x, 0.0f, 0.0f));
    }
}