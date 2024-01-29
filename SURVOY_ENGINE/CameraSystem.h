#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include "System.h"
#include "EntityManager.h"
#include "CameraComponent.h"
#include "VectorHelpers.h"

namespace ECS {
	class CameraSystem : public System
	{	
	public:
		const float SPEED = 2.f;
		const float MOUSE_SENSITIVITY = 20.f;

		CameraSystem(EntityManager& manager);

		void Load(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities) override;
		void Physics(float deltaTime, std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities) override;
		void Renders(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities);
		void Unload(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities);

	private:
		EntityManager& _entityManager;
		float _acceleration;

		void _updateVectors(glm::vec3& forward, glm::vec3& up, glm::vec3& right, float& yaw, float& pitch);
		void _mouseLook(float deltaTime, float& yaw, float& pitch, float& mouseX, float& mouseY);
		void _move(float deltaTime, glm::vec3& position, const glm::vec3& forward, const glm::vec3& right);
	};

}
