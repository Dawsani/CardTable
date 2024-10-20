#ifndef CARD_H
#define CARD_H

#include "GameObject.h"
#include "HitBox.h"

class Card : public GameObject {
    public:
        Card(ShaderProgram* pShaderProgram, ShaderProgram* pScreenSpaceShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, glm::vec2 hitBoxSize);
        HitBox* getHitBox() { return pHitBox; }

        void sendToHand() { inHand = true; }
        void sendToBoard() { inHand = false; }

        void draw(Camera *pCamera) override;
    private:
        HitBox* pHitBox;
        ShaderProgram* pScreenSpaceShaderProgram;
        bool inHand;
};

#endif