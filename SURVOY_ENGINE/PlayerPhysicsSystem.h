#pragma once

#include "System.h"
#include "EntityManager.h"
#include "Physics.h"

namespace ECS {
	class PlayerPhysicsSystem : public System
	{
	public:
		PlayerPhysicsSystem(EntityManager& entityManager, Physics& physics);

		void UpdateOnFixedTimestep(float deltaTime) override;

	private:
		EntityManager& _entityManager;
		Physics& _physics;
	};
}
