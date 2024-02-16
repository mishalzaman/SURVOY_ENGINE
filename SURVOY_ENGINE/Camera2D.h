#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ENGINE {
	class Camera2D
	{
	public:
		Camera2D(
			const float width,
			const float height
		);

		glm::mat4 Mat4Projection();

	private:
		const float			_fScreenWidth;
		const float			_fScreenHeight;
	};
}
