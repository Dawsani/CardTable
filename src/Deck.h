#ifndef DECK_H
#define DECK_H

#include <deque>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "GameObject.h"
#include "Card.h"

class Deck : public GameObject {
public:
    Deck(class Engine* pEngine, ShaderProgram* pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, std::deque<Card*> cards);

    void onLeftClick() override;

    float checkRayCollision(glm::vec3 rayOrigin, glm::vec3 rayDirection) override;

    void shuffle();
private:
    std::deque<Card*> cards;
};

#endif