#include "HitBox.h"
#include "GameObject.h"

HitBox::HitBox(GameObject *pGameObject, glm::vec2 size)
{
    this->pGameObject = pGameObject;
    this->size = size;
}

bool HitBox::CheckCollision(HitBox other)
{
    glm::vec2 position = glm::vec2(pGameObject->getPosition().x, pGameObject->getPosition().z);
    glm::vec2 otherPosition = glm::vec2(other.getGameObject()->getPosition().x, other.getGameObject()->getPosition().z);

    if (position.x < otherPosition.x + other.getSize().x &&
		position.x + size.x > otherPosition.x &&
		position.y < otherPosition.y + other.getSize().y &&
		position.y + size.y > otherPosition.y) {
		return true;
	}

	return false;
}
