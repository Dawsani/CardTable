#include <stack>
#include "GameObject.h"
#include "Card.h"

class Deck : public GameObject {
public:
    Deck(ShaderProgram* pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, std::stack<Card*> cards);

    Card* drawCard();
    void shuffle();
private:
    std::stack<Card*> cards;
};