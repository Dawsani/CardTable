#include "Deck.h"

Deck::Deck(ShaderProgram *pShaderProgram, unsigned int vaoHandle, unsigned int textureHandle, std::vector<Card *> cards) : GameObject(pShaderProgram, vaoHandle, textureHandle)
{
    this->cards = cards;
}