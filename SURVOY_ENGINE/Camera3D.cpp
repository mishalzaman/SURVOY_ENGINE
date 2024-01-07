#include "Camera3D.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BAE::Camera3D::Camera3D(glm::vec3 position, float screenWidth, float screenHeight):
    _forward(glm::vec3(0)),
    _right(glm::vec3(0)),
    _up(glm::vec3(0)),
    _movementSpeed(0.001f)
{
    _position = position;
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    _yaw = YAW;
    _pitch = PITCH;

    _velocity = glm::vec3(0.0f);  // Initialize velocity to zero
    _acceleration = 0.0009f;       // Set a suitable acceleration rate
    _maxSpeed = 0.009f;            // Set a maximum speed

    _updateCameraVectors();
}

glm::mat4 BAE::Camera3D::ViewMat4()
{
    return glm::lookAt(_position, _position + _forward, _up);
}

glm::mat4 BAE::Camera3D::ProjectionMat4()
{
    return glm::perspective(
    	glm::radians(60.0f),
    	(float)_screenWidth / (float)_screenHeight,
    	0.1f,
    	1000.0f
    );
}

void BAE::Camera3D::TankMovement(SDL_Event& event, float deltaTime)
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    // Accelerate or decelerate based on input
    if (keystate[SDL_SCANCODE_W]) {
        _velocity += _forward * _acceleration * deltaTime;
    }
    else if (keystate[SDL_SCANCODE_S]) {
        _velocity -= _forward * _acceleration * deltaTime;
    }
    else {
        // Apply deceleration
        _velocity *= 0.95f;  // Adjust this value for smoother deceleration
    }

    // Clamp the velocity to the maximum speed
    if (glm::length(_velocity) > _maxSpeed) {
        _velocity = glm::normalize(_velocity) * _maxSpeed;
    }

    if (keystate[SDL_SCANCODE_A]) {
        _yaw -= 2.0f;
    }
    if (keystate[SDL_SCANCODE_D]) {
        _yaw += 2.0f;
    }

    _position += _velocity * deltaTime;

    _updateCameraVectors();
}


void BAE::Camera3D::_updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _forward = glm::normalize(front);

    _right = glm::normalize(glm::cross(_forward, _worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    _up = glm::normalize(glm::cross(_right, _forward));
}
