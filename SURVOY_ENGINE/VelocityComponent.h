#pragma once

#include <glm/glm.hpp>

namespace ECS {
	struct VelocityComponent
	{
	public:
		VelocityComponent() :
			Velocity(0.f), Direction(glm::vec3(0)) {};

		float Velocity;
		glm::vec3 Direction;
	};
}
