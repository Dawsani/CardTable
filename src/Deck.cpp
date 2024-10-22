#include "Deck.h"
#include "Engine.h"

Deck::Deck(Engine* pEngine, ShaderProgram *pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, std::deque<Card *> cards) : GameObject(pEngine, pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
{
    this->cards = cards;
    setScale(glm::vec3(0.63f, cards.size() * 0.005, 0.88f));

    shuffle();
}

void Deck::onLeftClick()
{
    if (cards.empty()) {
        return;
    }
    
    Card* c = cards.front();
    c->select();
    pEngine->addGameObject(c);
    cards.pop_front();
    setScale(glm::vec3(0.63f, cards.size() * 0.005, 0.88f));
}

float Deck::checkRayCollision(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
    // just check the top plane of the deck, for now
    glm::vec3 normal = glm::vec3(0.0f, -1.0f, 0.0f);
    float d = scale.y;
    float t = - (glm::dot(rayOrigin, normal) + d) / glm::dot(rayDirection, normal);
    glm::vec3 intersectionPoint = rayOrigin + rayDirection * t;

    if (intersectionPoint.x < position.x + scale.x && intersectionPoint.x > position.x &&
        intersectionPoint.z < position.z + scale.z && intersectionPoint.z > position.z) {
            return glm::distance(intersectionPoint, pEngine->getCamera()->getPosition());
    }

    return -1;
}

void Deck::shuffle()
{
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(cards.begin(), cards.end(), rng);
}
