#pragma once

#include <utility>
#include "System.h"
#include "IObserver.h"
#include "EntityManager.h"
#include "Physics.h"
#include "EventManager.h"
#include "InputMouseRelXYEvent.h"
#include "VectorHelpers.h"
#include "CameraMouseComponent.h"
#include "ScreenDimensionsComponent.h"
#include "CameraMatricesComponent.h"
#include "CameraOrientationComponent.h"
#include "CameraPositionEvent.h"
#include "CameraViewProjectionEvent.h"
#include "CameraPositionEvent.h"
#include "TransformComponent.h"

namespace ECS {
	class CameraThirdPersonSystem : public System, public IObserver
	{
	public:
		const float SPEED = 2.f;
		const float MOUSE_SENSITIVITY = 10.f;
		const float ACCELERATION = 2.f;
		const float DISTANCE_TO_TARGET = 6.f;
		const float UPWARD_OFFSET = -4.f;

		CameraThirdPersonSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
		~CameraThirdPersonSystem();

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

		void _orbit(float deltaTime, float& yaw, float& pitch, float& mouseX, float& mouseY, glm::vec3& position, glm::vec3& forward, glm::vec3& right, glm::vec3& up, glm::vec3 target);
		void _move(float deltaTime, glm::vec3& position, const glm::vec3& forward, const glm::vec3& right);
	};
}
