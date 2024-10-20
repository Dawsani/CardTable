#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        Camera(glm::vec3 position, float fov, glm::vec2 windowSize);

        void Pan(glm::vec2 panMovement);
        void moveForward(float movement);

        void updateProjectionMatrices(glm::vec2 windowSize);

        glm::vec3 getPosition() { return position; }
        glm::mat4 getViewMatrix() { return viewMatrix; }
        glm::mat4 getProjectionMatrix() { return projectionMatrix; }
        glm::mat4 getOrthographicProjectionMatrix() { return orthographicProjectionMatrix; }

        void setMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
        void setZoomSpeed(float zoomSpeed) { this->zoomSpeed = zoomSpeed; }
    private:

        void updateViewMatrix();

        float fov;

        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 orthographicProjectionMatrix;

        glm::vec3 position;
        glm::vec3 lookAtPoint;
        glm::vec3 upVector;
        float moveSpeed;  
        float zoomSpeed;
};

#endif