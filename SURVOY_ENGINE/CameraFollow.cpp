#include "CameraFollow.h"
#include <utility>

BAE::CameraFollow::CameraFollow(float screenWidth, float screenHeight):
	Camera3DBase(screenWidth, screenHeight),
	_target(glm::vec3(0)),
	_acceleration(2.f),
    _distanceToTarget(DISTANCE_TO_FOLLOW)
{
}

void BAE::CameraFollow::Update(glm::vec3 target, float deltaTime)
{
    _target = target;

    _orbit(deltaTime);
    _move();
}

void BAE::CameraFollow::SetMouseOffsets(float x, float y)
{
    _MouseX = x;
    _MouseY = y;
}

void BAE::CameraFollow::Orbit(float deltaTime)
{
    // Sensitivity scaling
    float sensitivity = MOUSE_SENSITIVITY * deltaTime;

    // Adjust yaw and pitch based on mouse movement
    _Yaw += _MouseX * sensitivity;
    _Pitch += _MouseY * sensitivity;

    // Clamp the pitch
    _Pitch = std::max(std::min(_Pitch, 89.0f), -89.0f);

    // Convert yaw and pitch to radians
    float yawRad = glm::radians(_Yaw);
    float pitchRad = glm::radians(_Pitch);

    // Recalculate the camera position
    glm::vec3 offset;
    offset.x = _distanceToTarget * cos(pitchRad) * cos(yawRad);
    offset.y = _distanceToTarget * sin(pitchRad);
    offset.z = _distanceToTarget * cos(pitchRad) * sin(yawRad);

    _Position = _target - offset;

    // Update the camera's forward vector
    _Forward = glm::normalize(_target - _Position);

    // Recalculate right and up vector
    _Right = glm::normalize(glm::cross(_Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    _Up = glm::normalize(glm::cross(_Right, _Forward));

    // Reset mouse offsets
    _MouseX = 0;
    _MouseY = 0;
}

void BAE::CameraFollow::Move()
{
    // Convert yaw and pitch to radians
    float yawRad = glm::radians(_Yaw);
    float pitchRad = glm::radians(_Pitch);

    // Calculate the offset from the target in world space
    glm::vec3 offset;
    offset.x = _distanceToTarget * cos(pitchRad) * cos(yawRad);
    offset.y = _distanceToTarget * sin(pitchRad);
    offset.z = _distanceToTarget * cos(pitchRad) * sin(yawRad);

    // Set the camera's position
    _Position = _target - offset;

    // Update the camera's forward vector
    _Forward = glm::normalize(_target - _Position);

    // Assuming _Up is already set correctly in Camera3DBase or elsewhere
    _Right = glm::normalize(glm::cross(_Forward, _Up));
    _Up = glm::normalize(glm::cross(_Right, _Forward));
}

void BAE::CameraFollow::Target(glm::vec3 target)
{
    _target = target;
}

void BAE::CameraFollow::_move()
{
    // Convert yaw and pitch to radians
    float yawRad = glm::radians(_Yaw);
    float pitchRad = glm::radians(_Pitch);

    // Calculate the offset from the target in world space
    glm::vec3 offset;
    offset.x = _distanceToTarget * cos(pitchRad) * cos(yawRad);
    offset.y = _distanceToTarget * sin(pitchRad);
    offset.z = _distanceToTarget * cos(pitchRad) * sin(yawRad);

    // Set the camera's position
    _Position = _target - offset;

    // Update the camera's forward vector
    _Forward = glm::normalize(_target - _Position);

    // Assuming _Up is already set correctly in Camera3DBase or elsewhere
    _Right = glm::normalize(glm::cross(_Forward, _Up));
    _Up = glm::normalize(glm::cross(_Right, _Forward));
}

void BAE::CameraFollow::_orbit(float deltaTime)
{
    // Sensitivity scaling
    float sensitivity = MOUSE_SENSITIVITY * deltaTime;

    // Adjust yaw and pitch based on mouse movement
    _Yaw += _MouseX * sensitivity;
    _Pitch += _MouseY * sensitivity;

    // Clamp the pitch
    _Pitch = std::max(std::min(_Pitch, 89.0f), -89.0f);

    // Convert yaw and pitch to radians
    float yawRad = glm::radians(_Yaw);
    float pitchRad = glm::radians(_Pitch);

    // Recalculate the camera position
    glm::vec3 offset;
    offset.x = _distanceToTarget * cos(pitchRad) * cos(yawRad);
    offset.y = _distanceToTarget * sin(pitchRad);
    offset.z = _distanceToTarget * cos(pitchRad) * sin(yawRad);

    _Position = _target - offset;

    // Update the camera's forward vector
    _Forward = glm::normalize(_target - _Position);

    // Recalculate right and up vector
    _Right = glm::normalize(glm::cross(_Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    _Up = glm::normalize(glm::cross(_Right, _Forward));

    // Reset mouse offsets
    _MouseX = 0;
    _MouseY = 0;
}

