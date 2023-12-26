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
	// Calculate the current direction from the camera to the target
	_direction = glm::normalize(_target - _position);

	return glm::lookAt(
		_position,        // Camera's current position
		_position + _direction,  // The point to look at
		UP_VECTOR         // The "up" direction for the camera
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

void BAE::Camera3D::UpdateOrbit(SDL_Event& event, float deltaTime)
{
    // Check for key presses
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    float speed = 0.001;

    if (keystate[SDL_SCANCODE_W]) {
        _verticalAngle += _orbitSpeedV * deltaTime * speed;
    }
    if (keystate[SDL_SCANCODE_S]) {
        _verticalAngle -= _orbitSpeedV * deltaTime * speed;
    }
    if (keystate[SDL_SCANCODE_A]) {
        _horizontalAngle += _orbitSpeedH * deltaTime * speed;
    }
    if (keystate[SDL_SCANCODE_D]) {
        _horizontalAngle -= _orbitSpeedH * deltaTime * speed;
    }

    // Calculate new position
    _position.x = _target.x + _orbitDistance * cos(_verticalAngle) * sin(_horizontalAngle);
    _position.y = _target.y + _orbitDistance * sin(_verticalAngle);
    _position.z = _target.z + _orbitDistance * cos(_verticalAngle) * cos(_horizontalAngle);

    // Update camera direction and right vector
    _direction = glm::normalize(_target - _position);
    _right = glm::normalize(glm::cross(_direction, UP_VECTOR));
    _up = glm::cross(_right, _direction);
}
