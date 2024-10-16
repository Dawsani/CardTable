#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        Camera(glm::vec3 position);

        void Pan(glm::vec2 panMovement);
        void moveForward(float movement);

        glm::mat4 getViewMatrix() { return viewMatrix; }
        glm::mat4 getProjectionMatrix() { return projectionMatrix; }

        void setMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
        void setZoomSpeed(float zoomSpeed) { this->zoomSpeed = zoomSpeed; }
    private:

        void updateViewMatrix();

        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        glm::vec3 position;
        glm::vec3 lookAtPoint;
        glm::vec3 upVector;
        float moveSpeed;  
        float zoomSpeed;
};

#endif