#include "PlayerPhysicsSystem.h"

ECS::PlayerPhysicsSystem::PlayerPhysicsSystem(EntityManager& entityManager, Physics& physics):
	_entityManager(entityManager), _physics(physics)
{
}

void ECS::PlayerPhysicsSystem::UpdateOnFixedTimestep(float deltaTime)
{
}
