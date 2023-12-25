#include "Camera3D.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BAE::Camera3D::Camera3D()
{
	_position = glm::vec3(0.0f, 0.0f, 3.0f);
	_target = glm::vec3(0.0f, 0.0f, 0.0f);
	_direction = glm::normalize(_position - _target);
	_right = glm::normalize(glm::cross(UP_VECTOR, _direction));
	_up = glm::cross(_direction, _right);
	_fov = glm::radians(60.0f);
}

glm::mat4 BAE::Camera3D::View()
{
	return glm::lookAt(
		_position,
		_position + FRONT_VECTOR,
		UP_VECTOR
	);
}

glm::mat4 BAE::Camera3D::Projection()
{
	return glm::perspective(
		_fov,
		(float)Defaults::BASE_SCREEN_WIDTH / (float)Defaults::BASE_SCREEN_HEIGHT,
		0.1f,
		1000.0f
	);
}
