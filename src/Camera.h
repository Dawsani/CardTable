#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GL/gl.h>

class Camera {
    public:
        void move(glm::vec3 movement);

        void computeViewMatrix();

        void updateViewMatrix();
        
    private:
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        glm::vec3 cameraPosition;
        glm::vec3 cameraDirection;
        glm::vec3 cameraLookAtPoint;
        glm::vec3 cameraUpVector;
};