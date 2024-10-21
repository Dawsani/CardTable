#ifndef CARD_H
#define CARD_H

#include "GameObject.h"
#include "HitBox.h"

class Card : public GameObject {
    public:
        Card(class Engine* pEngine, ShaderProgram* pShaderProgram, ShaderProgram* pScreenSpaceShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, glm::vec2 hitBoxSize);
        
        void onLeftClick() override;
        void onLeftRelease() override;
        void update() override;

        void select();
        
        bool check2DPointCollision(glm::vec2 point);
        HitBox* getHitBox() { return pHitBox; }

        void toggleIsTapped();
        void sendToHand() { inHand = true; }
        void sendToBoard() { inHand = false; }

        void draw(Camera *pCamera) override;
    private:
        HitBox* pHitBox;
        ShaderProgram* pScreenSpaceShaderProgram;
        bool inHand;
        bool isTapped;
        bool isSelected;
};

#endif