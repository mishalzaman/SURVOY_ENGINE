#pragma once

/*
KINEMATIC CHARACTER CONTROLLER

States:
	IDLE
	MOVEING

Contact:
	GROUND
	SLOPE
	WALL
*/

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/norm.hpp>

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
		const float GROUND_TEST_OFFSET = 0.07f;

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

		void _updateVectors();

		/*/==============================
		POSITION UPDATES
		================================*/
		void _updateKinematicPosition();
		void _updateGhostObjectPosition();
		void _updateEntityPosition();

		/*/==============================
		MOVEMENT UPDATES
		================================*/
		void _handleGravity(float deltaTime);
		void _move(float deltaTime);

		/*/==============================
		PHYSICS TESTS
		================================*/
		bool _isOnGround();
		bool _isOnSlope();
		bool _IsNextToWall();
	};
}
