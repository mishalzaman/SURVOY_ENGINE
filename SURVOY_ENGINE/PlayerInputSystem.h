#pragma once

#include "System.h"
#include "EntityManager.h"

#include "OrientationComponent.h"
#include "MovementAttributesComponent.h";
#include "GhostObjectCapsuleComponent.h"

#include "VectorHelpers.h"

namespace ECS {
	class PlayerInputSystem : public System
	{
	public:
		PlayerInputSystem(EntityManager& entityManager);

		void UpdateOnFixedTimestep(float deltaTime) override;

	private:
		EntityManager& _entityManager;

		void _turn(float deltaTime);
		void _move(float deltaTime);
		void _applyGravity(float deltaTime);
		void _updateVectors();
		void _updateGhostObjectPosition();

		glm::vec3 _verticalVelocity;
	};
}
