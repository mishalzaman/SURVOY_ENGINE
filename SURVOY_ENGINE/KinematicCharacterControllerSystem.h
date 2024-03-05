#pragma once

#include <iostream>

// ECS
#include "System.h"
#include "EntityManager.h"
#include "OrientationComponent.h"
#include "KinematicCapsulePhysicsBodyComponent.h"

// Engine
#include "Physics.h"
#include "PhysicsCustomConvexResultCallback .h"

// Helpers
#include "VectorHelpers.h"

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace ECS {
	class KinematicCharacterControllerSystem : public System
	{
	const glm::vec3 GRAVITY = glm::vec3(0, -0.0098f, 0);
	const float GHOST_OBJECT_SCALE = 1.015f;

	public:
		KinematicCharacterControllerSystem(EntityManager& entityManager, Physics& physics);
		~KinematicCharacterControllerSystem();

		void Load() override;
		void UpdateOnFixedTimestep(float deltaTime) override;
		void Unload() override;
	private:
		EntityManager& _entityManager;
		Physics& _physics;

		glm::vec3 _velocity;
		float _acceleration;

		bool _isOnGround();
		void _handleGravity(KinematicCapsulePhysicsBodyComponent& kinematic, float deltaTime);

		// physics
		void _createGhostObject();
		btPairCachingGhostObject* _ghostObject;
	};
}
