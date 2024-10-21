#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Camera.h"
#include "ShaderProgram.h"

class GameObject {
public:
    GameObject (class Engine* pEngine, ShaderProgram* pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle);

    virtual void onLeftClick() {};
    virtual void onLeftRelease() {};
    virtual void onRightClick() {};
    virtual void onRightRelease() {};
    virtual void onHover() {};

    virtual void update() {};

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

    glm::vec3 getPosition() { return position; }
    glm::vec3 getRotation() { return rotation; }
    glm::vec3 getScale() { return scale; }

    virtual void draw(Camera* pCamera);
protected:
    class Engine* pEngine;
    ShaderProgram* pShaderProgram;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    unsigned int textureHandle;
    unsigned int vaoHandle;
    unsigned int numVAOPoints;

    glm::mat4 modelMatrix;

    void updateModelMatrix();

    bool isSelected = false;
};

#endif