#pragma once

#include <glm/glm.hpp>
#include "System.h"
#include "IObserver.h"
#include "EntityManager.h"
#include "Physics.h"
#include "EventManager.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "OrientationComponent.h"
#include "VectorHelpers.h"
#include "DynamicCapsulePhysicsBodyComponent.h"
#include "CameraYawEvent.h"
#include "CharacterControllerPositionEvent.h"

namespace ECS {
	class CharacterControllerSystem : public System, public IObserver
	{
	public:
		const float SPEED = 48.f;
		const float ACCELERATION = 4.f;

		CharacterControllerSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
		~CharacterControllerSystem();

		void onNotify(const Event& event) override;

		void Load() override;
		void UpdateOnFixedTimestep(float deltaTime) override;
		void UpdateOnVariableTimestep() override;
		void Render() override;

	private:
		EventManager& _eventManager;
		EntityManager& _entityManager;
		Physics& _physics;

		void _updateInput(float deltaTime, const glm::vec3& forward, const glm::vec3& right, float& velocity, glm::vec3& direction);
		void _updateYaw(float yaw);
		void _updateVectors(const float yaw, glm::vec3& forward, glm::vec3& right, glm::vec3& up);
		void _updatePhysics(ECS::DynamicCapsulePhysicsBodyComponent& dynamic, const glm::vec3& direction, const float& velocity);
	};
}
