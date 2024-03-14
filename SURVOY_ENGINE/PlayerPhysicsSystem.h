#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp> // For angle computation

#include "System.h"
#include "EntityManager.h"
#include "Physics.h"

#include "GhostObjectCapsuleComponent.h"
#include "MovementAttributesComponent.h"
#include "OrientationComponent.h"
#include "KinematicCapsulePhysicsBodyComponent.h"


namespace ECS {
	class PlayerPhysicsSystem : public System
	{
	public:
		PlayerPhysicsSystem(EntityManager& entityManager, Physics& physics);

		void UpdateOnFixedTimestep(float deltaTime) override;

	private:
		EntityManager& _entityManager;
		Physics& _physics;

		btVector3 _originalPosition;

		bool _contacts();
		bool _onGround();
		glm::vec3 _projectOnPlane(
			glm::vec3 velocity,
			glm::vec3 normal
		);
		float _calculateNormalAngle(glm::vec3 normal);

		void _updateGhostObjectPosition(glm::vec3 velocity);
		btVector3 _getGhostObjectOriginalPosition();
	};
}
