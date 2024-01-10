#include "Camera3D.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

BAE::Camera3D::Camera3D(glm::vec3 position, float screenWidth, float screenHeight):
    _forward(glm::vec3(0)),
    _right(glm::vec3(0)),
    _up(glm::vec3(0)),
    _position(position),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight)
{
    _cacheVectors();
}

glm::mat4 BAE::Camera3D::ViewMat4()
{
    return BAE::CameraHelpers::ViewMat4(_position, _forward, _up);
}

glm::mat4 BAE::Camera3D::ProjectionMat4()
{
    return CameraHelpers::ProjectionMat4(_screenWidth, _screenHeight, 60.0f);
}

void BAE::Camera3D::SetPosition(glm::vec3 position)
{
    _position = position;
}

void BAE::Camera3D::SetForward(glm::vec3 forward)
{
    _forward = forward;
}

void BAE::Camera3D::Update()
{
    _cacheVectors();
}

void BAE::Camera3D::_cacheVectors()
{
    _right = BAE::CameraHelpers::RightVec3(_forward);
    _up = BAE::CameraHelpers::UpVec3(_forward, _right);
}
