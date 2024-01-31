#pragma once

#include <glm/glm.hpp>

namespace ECS {
    struct CharacterControllerComponent {
		glm::vec3 Position;
		glm::mat4 Transformation;
		float Yaw;
		float Pitch;
		glm::vec3 Forward;
		glm::vec3 Right;
		glm::vec3 Up;

		glm::vec3 Direction;
		float Acceleration = 4.f;
		float MovementSpeed = 32.f;
		float Velocity;

		CharacterControllerComponent(glm::mat4 transformation) :
			Transformation(transformation) {};
    };
}
