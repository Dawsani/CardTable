#include "Deck.h"

Deck::Deck(ShaderProgram *pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, std::deque<Card *> cards) : GameObject(pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
{
    this->cards = cards;
    setScale(glm::vec3(1.0f, cards.size(), 1.0f));

    shuffle();
}

Card *Deck::drawCard()
{
    if (cards.size() <= 0) {
        return nullptr;
    }
    Card* c = cards.front();
    cards.pop_front();
    setScale(glm::vec3(1.0f, cards.size(), 1.0f));
    return c;
}

void Deck::shuffle()
{
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(cards.begin(), cards.end(), rng);
}
