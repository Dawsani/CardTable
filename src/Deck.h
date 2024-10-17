#include <vector>
#include "GameObject.h"
#include "Card.h"

class Deck : public GameObject {
public:
    Deck(ShaderProgram* pShaderProgram, unsigned int vaoHandle, unsigned int textureHandle, std::vector<Card*> cards);
private:
    std::vector<Card*> cards;
};