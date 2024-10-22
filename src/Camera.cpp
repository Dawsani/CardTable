#include "Camera.h"

Camera::Camera(glm::vec3 position, float fov, glm::vec2 windowSize) {
    this->position = position;
    this->fov = fov;
    lookAtPoint = glm::vec3(0.0f, 0.0f, 0.0f);
    upVector = glm::vec3(0.0f, 0.0f, 1.0f);
    moveSpeed = 0.01f;
    zoomSpeed = 0.1f;
    updateViewMatrix();
    updateProjectionMatrices(windowSize);
}

void Camera::updateProjectionMatrices(glm::vec2 windowSize)
{
    projectionMatrix = glm::perspective(glm::radians(fov), windowSize.x / windowSize.y, 0.1f, 100.0f);
    orthographicProjectionMatrix = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y, -1.0f, 1.0f);
}

void Camera::Pan(glm::vec2 panMovement)
{
    glm::vec3 movement = glm::vec3(panMovement.x, -panMovement.y, 0.0f);
    glm::vec3 finalMovement = moveSpeed * movement;
    position += finalMovement;
    lookAtPoint += finalMovement;
    updateViewMatrix();
}

void Camera::moveForward(float movement)
{
    glm::vec3 direction = glm::normalize(position - lookAtPoint);
    position += zoomSpeed * movement * direction;
    updateViewMatrix();
}

void Camera::updateViewMatrix()
{
    viewMatrix = glm::lookAt(position, lookAtPoint, upVector);
}
