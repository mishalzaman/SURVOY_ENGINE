#pragma once

#include <iostream>

// ECS
#include "System.h"
#include "EntityManager.h"
#include "OrientationComponent.h"
#include "KinematicCapsulePhysicsBodyComponent.h"

// Engine
#include "Physics.h"

// Helpers
#include "VectorHelpers.h"

namespace ECS {
	class KinematicCharacterControllerSystem : public System
	{
	const float GRAVITY = -0.98f;

	public:
		KinematicCharacterControllerSystem(EntityManager& entityManager, Physics& physics);

		void Load() override;
		void UpdateOnFixedTimestep(float deltaTime) override;
		void Unload() override;
	private:
		EntityManager& _entityManager;
		Physics& _physics;

		bool _isOnGround(KinematicCapsulePhysicsBodyComponent& kinematic);

		void _handleGravity(KinematicCapsulePhysicsBodyComponent& kinematic, float deltaTime);
	};
}
