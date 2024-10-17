#include "Deck.h"

Deck::Deck(ShaderProgram *pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, std::vector<Card *> cards) : GameObject(pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
{
    this->cards = cards;
}