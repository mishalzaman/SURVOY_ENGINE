#pragma once

#include "System.h"
#include "EntityManager.h"

#include "OrientationComponent.h"
#include "MovementAttributesComponent.h"
#include "GhostObjectCapsuleComponent.h"

namespace ECS {
	class PlayerOutputSystem : public System
	{
	public:
		PlayerOutputSystem(EntityManager& entityManager);

		void UpdateOnFixedTimestep(float deltaTime) override;

	private:
		EntityManager& _entityManager;
	};
}
