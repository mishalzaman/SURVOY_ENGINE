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
#include "GhostObjectCapsuleComponent.h"
#include "MovementAttributesComponent.h"

// Engineawdawdawd
#include "Physics.h"
#include "PhysicsCustomConvexResultCallback .h"

// Helpers
#include "VectorHelpers.h"

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


		void _resetVelocity();

		/*/==============================
		POSITION UPDATES
		================================*/
		void _updateKinematicPosition(glm::vec3 displacement);
		void _updateGhostObjectPosition();
		void _updateEntityPosition();

		/*/==============================
		MOVEMENT UPDATES
		================================*/
		void _handleGravity(float deltaTime);

		/*/==============================
		PHYSICS TESTS
		================================*/
		bool _isOnGround();
		bool _isOnSlope();
		bool _IsNextToWall();
	};
}
