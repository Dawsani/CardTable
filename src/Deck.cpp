#include "Deck.h"

Deck::Deck(ShaderProgram *pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, std::stack<Card *> cards) : GameObject(pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
{
    this->cards = cards;
}

Card *Deck::drawCard()
{
    Card* c = cards.top();
    cards.pop();
    return c;
}
