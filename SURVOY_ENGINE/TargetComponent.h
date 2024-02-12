#pragma once

#include <glm/glm.hpp>

namespace ECS {
	struct TargetComponent
	{
	public:
		TargetComponent() :
			Target(glm::vec3(0)) {};

		glm::vec3 Target;
	};
}
#pragma once
