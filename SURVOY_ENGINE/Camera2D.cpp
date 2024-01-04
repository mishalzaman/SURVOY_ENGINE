#include "Camera2D.h"

BAE::Camera2D::Camera2D(const float width, const float height):
	_screenWidth(width),
	_screenHeight(height)
{
}

glm::mat4 BAE::Camera2D::Projection()
{
	return glm::ortho(0.0f, static_cast<float>(_screenWidth), static_cast<float>(_screenHeight), 0.0f, -1.0f, 1.0f);
}
