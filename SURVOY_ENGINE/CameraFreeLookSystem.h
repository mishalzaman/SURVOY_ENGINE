#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include "System.h"
#include "EntityManager.h"
#include "VectorHelpers.h"
#include "Physics.h"
#include "EventManager.h"
#include "CameraViewProjectionEvent.h"
#include "IObserver.h"
#include "InputMouseRelXYEvent.h"
#include "RenderTargetDimensionsComponent.h"
#include "CameraMatricesComponent.h"
#include "CameraMouseComponent.h"
#include "CameraPositionEvent.h"
#include "OrientationComponent.h"
#include "CameraFOVComponent.h"

namespace ECS {
	class CameraFreeLookSystem : public System, public IObserver
	{	
	public:
		const float SPEED = 2.f;
		const float MOUSE_SENSITIVITY = 10.f;
		const float ACCELERATION = 2.f;

		CameraFreeLookSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
		~CameraFreeLookSystem();

		void onNotify(const Event& event) override;

		void Load() override;
		void UpdateOnFixedTimestep(float deltaTime) override;

	private:
		EventManager& _eventManager;
		EntityManager& _entityManager;
		Physics& _physics;

		void _updateVectors(glm::vec3& forward, glm::vec3& up, glm::vec3& right, float& yaw, float& pitch);
		void _mouseLook(float deltaTime, float& yaw, float& pitch, float& mouseX, float& mouseY);
		void _move(float deltaTime, glm::vec3& position, const glm::vec3& forward, const glm::vec3& right);
	};

}
