#include "glm/vec2.hpp"

class HitBox {
public:
    HitBox(class GameObject* pGameObject, glm::vec2 size);
    bool CheckCollision(HitBox* pOther);
    class GameObject* getGameObject() { return pGameObject; }
    glm::vec2 getSize() { return size; }
private:
    class GameObject* pGameObject;
    glm::vec2 size;
};