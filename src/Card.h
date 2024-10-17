#include "GameObject.h"
#include "HitBox.h"

class Card : public GameObject {
    public:
        Card(ShaderProgram* pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, glm::vec2 hitBoxSize);
        HitBox* getHitBox() { return pHitBox; }
    private:
        HitBox* pHitBox;
};