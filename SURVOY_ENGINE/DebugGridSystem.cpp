#include "DebugGridSystem.h"

ECS::DebugGridSystem::DebugGridSystem(EntityManager& entitySystem):
	_entityManager(entitySystem)
{
	_grid = std::make_unique<ENGINE::Grid>();
}

void ECS::DebugGridSystem::Load()
{
}

void ECS::DebugGridSystem::Render()
{
	ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(
		_entityManager.getIdByTag("Camera")
	);
	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
		_entityManager.getIdByTag("Camera")
	);

	if (matrices && orientation) {
		_grid->render(matrices->Projection, matrices->View, orientation->Position);
	}
}
