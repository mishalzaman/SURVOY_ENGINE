#include "Camera2D.h"

BAE::Camera2D::Camera2D(const float width, const float height):
	_fScreenWidth(width),
	_fScreenHeight(height)
{
}

glm::mat4 BAE::Camera2D::Mat4Projection()
{
	return glm::ortho(
		0.0f,
		static_cast<float>(_fScreenWidth),
		static_cast<float>(_fScreenHeight),
		0.0f,
		-1.0f,
		1.0f
	);
}
