#pragma once

#include <glm/glm.hpp>

namespace ECS {
	struct DirectionalLightComponent
	{
		glm::vec3 Position;

		DirectionalLightComponent(glm::vec3 position) :
			Position(position) {}
	};
}
