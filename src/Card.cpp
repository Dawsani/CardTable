#include "Card.h"

Card::Card(ShaderProgram *pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, glm::vec2 hitBoxSize) : GameObject(pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
{
    this->pHitBox = new HitBox(this, hitBoxSize);
}