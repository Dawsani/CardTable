#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Camera.h"
#include "ShaderProgram.h"

class GameObject {
public:
    GameObject(ShaderProgram* pShaderProgram, unsigned int vaoHandle, unsigned int textureHandle);

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

    void draw(Camera* pCamera);
private:
    ShaderProgram* pShaderProgram;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    unsigned int textureHandle;
    unsigned int vaoHandle;

    glm::mat4 modelMatrix;

    void updateModelMatrix();
};