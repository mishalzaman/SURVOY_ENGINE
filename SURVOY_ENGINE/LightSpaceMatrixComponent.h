#pragma once

#include <glm/glm.hpp>

namespace ECS {
	struct LightSpaceMatrixComponent
	{
	public:
		LightSpaceMatrixComponent() : LightSpaceMatrix(glm::mat4(1)) {}

		glm::mat4 LightSpaceMatrix;
	};
}
