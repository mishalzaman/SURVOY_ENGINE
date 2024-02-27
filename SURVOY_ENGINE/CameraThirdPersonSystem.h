#pragma once

#include <utility>
#define NDEBUG // Disables assertions
#include <cassert>
#include <glm/gtc/quaternion.hpp>       // For glm::quat
#include <glm/gtx/euler_angles.hpp>    // For glm::eulerAngles
#include <glm/gtx/rotate_vector.hpp>

#include "System.h"
#include "IObserver.h"
#include "EntityManager.h"
#include "Physics.h"
#include "EventManager.h"
#include "InputMouseRelXYEvent.h"
#include "VectorHelpers.h"
#include "CameraMouseComponent.h"
#include "RenderTargetDimensionsComponent.h"
#include "CameraMatricesComponent.h"
#include "CameraPositionEvent.h"
#include "CameraPositionEvent.h"
#include "TransformComponent.h"
#include "OrientationComponent.h"
#include "DynamicCapsulePhysicsBodyComponent.h"
#include "CameraYawEvent.h"
#include "TargetComponent.h"
#include "CharacterControllerPositionEvent.h"
#include "ActiveCameraComponent.h"

namespace ECS {
	class CameraThirdPersonSystem : public System, public IObserver
	{
	public:
		const float SPEED = 2.f;
		const float MOUSE_SENSITIVITY = 20.f;
		const float ACCELERATION = 4.f;
		const float DISTANCE_TO_TARGET = 5.f;
		const float UP_OFFSET = 0.8f;
		const float RIGHT_OFFSET = 0.4f;

		CameraThirdPersonSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
		~CameraThirdPersonSystem();

		void onNotify(const Event& event) override;

		void Load() override;
		void UpdateOnFixedTimestep(float deltaTime) override;
		void UpdateOnVariableTimestep() override;

	private:
		EventManager& _eventManager;
		EntityManager& _entityManager;
		Physics& _physics;

		void _follow(
			float deltaTime,
			float& yaw,
			float& pitch,
			float& mouseX,
			float& mouseY,
			glm::vec3& position,
			glm::vec3& forward,
			glm::vec3& right,
			glm::vec3& up,
			glm::vec3 target
		);
	};
}
