#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BAE {
	class Camera2D
	{
	public:
		Camera2D(const float width, const float height);

		glm::mat4 Projection();
	private:
		const float _screenWidth;
		const float _screenHeight;
	};
}
