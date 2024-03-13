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
	ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);

	// Get updated velocity
	btVector3 vel = btVector3(motion->Velocity.x, motion->Velocity.y, motion->Velocity.z);

	// Update ghost object
	btVector3 ghostPosition = ghost->GhostObject->getWorldTransform().getOrigin() + vel;
	ghost->GhostObject->getWorldTransform().setOrigin(ghostPosition);

	// Update entity
	orientation->Position = glm::vec3(ghostPosition.x(), ghostPosition.y(), ghostPosition.z());
}
