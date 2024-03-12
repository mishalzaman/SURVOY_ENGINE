#include "PlayerOutputSystem.h"

ECS::PlayerOutputSystem::PlayerOutputSystem(EntityManager& entityManager):
	_entityManager(entityManager)
{
}

void ECS::PlayerOutputSystem::UpdateOnFixedTimestep(float deltaTime)
{
	int e = _entityManager.getIdByTag("CharacterController");

	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
	ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);

	// update entity
	btVector3 ghostPosition = ghost->GhostObject->getWorldTransform().getOrigin();
	orientation->Position = glm::vec3(ghostPosition.x(), ghostPosition.y(), ghostPosition.z());
}
