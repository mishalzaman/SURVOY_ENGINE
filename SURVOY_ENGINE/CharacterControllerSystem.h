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

namespace ECS {
	class CharacterControllerSystem : public System, public IObserver
	{
	public:
		const float SPEED = 10.f;
		const float ACCELERATION = 2.f;

		CharacterControllerSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
		~CharacterControllerSystem();

		void onNotify(const Event& event) override;

		void Load() override;
		void Update() override;
		void Update(float deltaTime) override;
		void Renders() override;
		void Unload() override;

	private:
		EventManager& _eventManager;
		EntityManager& _entityManager;
		Physics& _physics;

		void _move(float deltaTime, const glm::vec3& forward, const glm::vec3& right, float& velocity, glm::vec3& direction);
	};
}