#include "Camera.h"

Camera::Camera(glm::vec3 position) {
    this->position = position;
    lookAtPoint = glm::vec3(0, 0, 0);
    upVector = glm::vec3(0.0f, 0.0f, -1.0f);
    moveSpeed = 0.01f;
    zoomSpeed = 0.1f;
    updateViewMatrix();
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Camera::Pan(glm::vec2 panMovement)
{
    glm::vec3 movement = glm::vec3(panMovement.x, 0.0f, panMovement.y);
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
