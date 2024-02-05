#pragma once

#include <glm/glm.hpp>

namespace ECS {
	struct CameraOrientationComponent
	{
		glm::vec3 Position;
		glm::vec3 Forward;
		glm::vec3 Up;
		glm::vec3 Right;
		float Yaw;
		float Pitch;

		CameraOrientationComponent(glm::vec3 position) :
			Position(position),
			Forward(glm::vec3(0, 0, 1)),
			Up(glm::vec3(0, 1, 0)),
			Right(glm::vec3(0)),
			Yaw(-90.0f),
			Pitch(-45.0f) {}
	};
}