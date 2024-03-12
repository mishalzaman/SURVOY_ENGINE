#include "PlayerOutputSystem.h"

ECS::PlayerOutputSystem::PlayerOutputSystem(EntityManager& entityManager):
	_entityManager(entityManager)
{
}

void ECS::PlayerOutputSystem::UpdateOnFixedTimestep(float deltaTime)
{
	int e = _entityManager.getIdByTag("CharacterController");

	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
	ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);
	ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);

	btTransform newTrans = ghost->GhostObject->getWorldTransform();
	btVector3 currentPosition = newTrans.getOrigin();
	btVector3 displacement = btVector3(motion->Velocity.x, motion->Velocity.y, motion->Velocity.z);
	btVector3 newPosition = currentPosition + displacement;

	// update ghost object
	newTrans.setOrigin(newPosition);
	ghost->GhostObject->setWorldTransform(newTrans);

	// update entity
	orientation->Position = glm::vec3(newPosition.x(), newPosition.y(), newPosition.z());
}
