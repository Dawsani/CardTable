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

    /**
     * @brief Checks if a ray intersects with the object
     * 
     * @param rayOrigin The starting point of the ray in 3D space.
     * @param rayDirection The direction vector of the ray.
     * @return Returns the distance between the camera and the collision point. >= 0 if there was a collision, -1 if no collision
     */
    virtual float checkRayCollision(glm::vec3 rayOrigin, glm::vec3 rayDirection) { return -1; };

    virtual bool checkVerticalCollision(GameObject* other);

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

    glm::vec3 getPosition() { return position; }
    glm::vec3 getRotation() { return rotation; }
    glm::vec3 getScale() { return scale; }
    bool getIsSelected() { return isSelected; }

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