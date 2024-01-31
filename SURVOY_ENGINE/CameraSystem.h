#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include "System.h"
#include "EntityManager.h"
#include "CameraComponent.h"
#include "VectorHelpers.h"
#include "MouseRelXY.h"

namespace ECS {
	class CameraSystem : public System
	{	
	public:
		const float SPEED = 2.f;
		const float MOUSE_SENSITIVITY = 10.f;

		CameraSystem();

		void Load(EntityManager& entityManager) override;
		void Update(EntityManager& entityManager) override;
		void Update(float deltaTime, EntityManager& entityManager) override;
		void Renders(EntityManager& entityManager) override;
		void Unload(EntityManager& entityManager) override;
		void UpdateVec3(EntityManager& entityManager, float x, float y, float z) override;

	private:
		float _acceleration;

		void _updateVectors(glm::vec3& forward, glm::vec3& up, glm::vec3& right, float& yaw, float& pitch);
		void _mouseLook(float deltaTime, float& yaw, float& pitch, float& mouseX, float& mouseY);
		void _move(float deltaTime, glm::vec3& position, const glm::vec3& forward, const glm::vec3& right);

		float _curRelX;
		float _curRelY;
	};

}
