#include "Camera3DBase.h"

BAE::Camera3DBase::Camera3DBase(float screenWidth, float screenHeight):
    _Forward(glm::vec3(0,0,1)),
    _Right(glm::vec3(0)),
    _Up(glm::vec3(0,1,0)),
    _Position(glm::vec3(0,1,1)),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight),
    _MouseX(0),
    _MouseY(0)
{
    UpdateVectors();
}

glm::mat4 BAE::Camera3DBase::View()
{
    return BAE::VectorHelpers::ViewMat4(_Position, _Forward, _Up);
}

glm::mat4 BAE::Camera3DBase::Projection()
{
    return VectorHelpers::ProjectionMat4(_screenWidth, _screenHeight, 60.0f);
}

void BAE::Camera3DBase::Position(glm::vec3 position)
{
    _Position = position;
}

void BAE::Camera3DBase::Forward(glm::vec3 forward)
{
    _Forward = forward;
}

void BAE::Camera3DBase::UpdateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
    front.y = sin(glm::radians(_Pitch));
    front.z = sin(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
    _Forward = glm::normalize(front);

    _Right = BAE::VectorHelpers::RightVec3(_Forward);

    _Up = BAE::VectorHelpers::UpVec3(_Forward, _Right);
}
