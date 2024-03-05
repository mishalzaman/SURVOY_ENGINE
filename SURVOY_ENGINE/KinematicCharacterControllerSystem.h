#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

// ECS
#include "System.h"
#include "EntityManager.h"
#include "OrientationComponent.h"
#include "KinematicCapsulePhysicsBodyComponent.h"

// Engineawdawdawd
#include "Physics.h"
#include "PhysicsCustomConvexResultCallback .h"

// Helpers
#include "VectorHelpers.h"

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace ECS {
	class KinematicCharacterControllerSystem : public System
	{
		const float SPEED = 10.f;
		const glm::vec3 GRAVITY = glm::vec3(0, -9.8f, 0);
		const float GHOST_OBJECT_SCALE = 1.015f;
		const float GROUND_TEST_OFFSET = 0.07f;
		const float DECELERATION = 5.0f;


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
		glm::vec3 _verticalVelocity;
		float _acceleration;

		void _move(float deltaTime);
		bool _isOnGround();
		void _handleGravity(KinematicCapsulePhysicsBodyComponent& kinematic, float deltaTime);
		void _resetVelocity();
		void _updateKinematicPosition(glm::vec3 displacement);

		// physics
		void _createGhostObject();
		btPairCachingGhostObject* _ghostObject;
	};
}
