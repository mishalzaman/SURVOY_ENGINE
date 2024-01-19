#include "CameraFreeLook.h"
#include <algorithm>

BAE::CameraFreeLook::CameraFreeLook(float screenWidth, float screenHeight) :
    Camera3DBase(screenWidth, screenHeight),
    _acceleration(2.f)
{

}

void BAE::CameraFreeLook::Update(float deltaTime)
{
    _mouseLook(deltaTime);

    Camera3DBase::UpdateVectors();

    _move(deltaTime);
}

void BAE::CameraFreeLook::SetMouseOffsets(float x, float y)
{
    _MouseX = x;
    _MouseY = y;
}

void BAE::CameraFreeLook::_move(float deltaTime)
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    float velocity = _acceleration * SPEED * deltaTime;

    if (keystate[SDL_SCANCODE_W]) {
        _Position += _Forward * velocity;
    }
    if (keystate[SDL_SCANCODE_S]) {
        _Position -= _Forward * velocity;
    }
    if (keystate[SDL_SCANCODE_A]) {
        _Position -= _Right * velocity;
    }
    if (keystate[SDL_SCANCODE_D]) {
        _Position += _Right * velocity;
    }
}

void BAE::CameraFreeLook::_mouseLook(float deltaTime)
{
    float xOffset = _MouseX;
    float yOffset = _MouseY;

    xOffset *= MOUSE_SENSITIVITY * deltaTime;
    yOffset *= MOUSE_SENSITIVITY * deltaTime;

    _Yaw += xOffset;
    _Pitch -= yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (_Pitch > 89.0f)
        _Pitch = 89.0f;
    if (_Pitch < -89.0f)
        _Pitch = -89.0f;

    _MouseX = 0;
    _MouseY = 0;
}
