#include "Deck.h"

Deck::Deck(ShaderProgram *pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, std::stack<Card *> cards) : GameObject(pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
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
    Card* c = cards.top();
    cards.pop();
    setScale(glm::vec3(1.0f, cards.size(), 1.0f));
    return c;
}

void Deck::shuffle()
{
    srand (time(NULL));
    std::vector<Card*> temp;
    while (cards.size() > 0) {
        temp.push_back(cards.top());
        cards.pop();
    }

    while (temp.size() > 0) {
        int i = rand() % temp.size();
        cards.push(temp[i]);
        temp.erase(temp.begin() + i);
    }
}
