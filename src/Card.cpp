#include "Card.h"

Card::Card(ShaderProgram *pShaderProgram, ShaderProgram* pScreenSpaceShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, glm::vec2 hitBoxSize) : GameObject(pShaderProgram, vaoHandle, numVAOPoints, textureHandle)
{
    this->pHitBox = new HitBox(this, hitBoxSize);
    this->pScreenSpaceShaderProgram = pScreenSpaceShaderProgram;
    inHand = false;
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
