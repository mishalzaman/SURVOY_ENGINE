#pragma once

#include <glm/glm.hpp>

namespace ECS {
	struct CameraMatricesComponent
	{
		glm::mat4 View;
		glm::mat4 Projection;

		CameraMatricesComponent(): View(glm::mat4(0)), Projection(glm::mat4(0)) {}
	};
}