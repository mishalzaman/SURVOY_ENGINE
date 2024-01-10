#include "Camera3D.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BAE::Camera3D::Camera3D(glm::vec3 position, float screenWidth, float screenHeight):
    _forward(glm::vec3(0)),
    _right(glm::vec3(0)),
    _up(glm::vec3(0))
{
    _position = position;
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    _yaw = YAW;
    _pitch = PITCH;

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

void BAE::Camera3D::LookAtTarget(glm::vec3 target)
{
    glm::vec3 direction = glm::normalize(target - _position);

    // Calculate pitch
    _pitch = glm::degrees(asin(direction.y));

    // Calculate yaw
    _yaw = glm::degrees(atan2(direction.z, direction.x));

    // Update camera vectors
    _updateCameraVectors();
}

void BAE::Camera3D::UpdateCamera(glm::vec3 position, glm::vec3 forward)
{
    _forward = glm::normalize(forward);

    _right = glm::normalize(glm::cross(
        _forward,
        glm::vec3(0.0f, 1.0f, 0.0f) // world up
    ));

    _up = glm::normalize(glm::cross(
        _right,
        _forward
    ));

    _position = position;
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
