#include <deque>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "GameObject.h"
#include "Card.h"

class Deck : public GameObject {
public:
    Deck(ShaderProgram* pShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, unsigned int textureHandle, std::deque<Card*> cards);

    Card* drawCard();
    void shuffle();
private:
    std::deque<Card*> cards;
};