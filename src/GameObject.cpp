#include "GameObject.h"

GameObject::GameObject(ShaderProgram* pShaderProgram, unsigned int vaoHandle, unsigned int textureHandle) {

    this->pShaderProgram = pShaderProgram;
    this->vaoHandle = vaoHandle;
    this->textureHandle = textureHandle;

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);

    updateModelMatrix();
}

void GameObject::setPosition(glm::vec3 position)
{
    this->position = position;
    updateModelMatrix();
}

void GameObject::setRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
    updateModelMatrix();
}

void GameObject::setScale(glm::vec3 scale)
{
    this->scale = scale;
    updateModelMatrix();
}

void GameObject::draw(Camera *pCamera)
{
    glm::mat4 viewMatrix = pCamera->getViewMatrix();
    glm::mat4 projectionMatrix = pCamera->getProjectionMatrix();
    glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    glProgramUniformMatrix4fv(pShaderProgram->getProgramHandle(), pShaderProgram->getMVPMatrixUniformLocation(), 1, GL_FALSE, &mvpMatrix[0][0]);
    glProgramUniform1i(pShaderProgram->getProgramHandle(), pShaderProgram->getTextureMapUniformLocation(), 0); // Set the texture to texture 0 for now, while we only have one texture

    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GameObject::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, scale);
}
