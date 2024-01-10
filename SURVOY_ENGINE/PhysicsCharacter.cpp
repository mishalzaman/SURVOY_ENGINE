 #include "PhysicsCharacter.h"

BAE::PhysicsCharacter::PhysicsCharacter(glm::vec3 position):
    _v3Forward(glm::vec3(0)),
    _fYaw(-90.0f),
    _fPitch(0.0f),
    _fMovementSpeed(0.001f),
    _v3Velocity(glm::vec3(0.0f)),
    _fAcceleration(0.09f),
    _fMaxSpeed(0.09f),
    _fMass(1.f),
    _v3Position(position)
{
    _setForward();
}

void BAE::PhysicsCharacter::MoveForward(float deltaTime)
{
    std::cout << "forward" << std::endl;
    _v3Velocity += _v3Forward * _fAcceleration * deltaTime;
}

void BAE::PhysicsCharacter::MoveBackward(float deltaTime)
{
    std::cout << "reverse" << std::endl;
    _v3Velocity -= _v3Forward * _fAcceleration * deltaTime;
}

void BAE::PhysicsCharacter::TurnLeft(float deltaTime)
{
    std::cout << "left" << std::endl;
    _fYaw -= TURN_SPEED;
}

void BAE::PhysicsCharacter::TurnRight(float deltaTime)
{
    std::cout << "right" << std::endl;
    _fYaw += TURN_SPEED;
}

void BAE::PhysicsCharacter::Decelerate()
{
    _v3Velocity *= 0.95f;
}

void BAE::PhysicsCharacter::Update()
{
    _clampVelocity();
    _v3Position += _v3Velocity;
    _setForward();
}

void BAE::PhysicsCharacter::Move(float deltaTime)
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    // Accelerate or decelerate based on input
    if (keystate[SDL_SCANCODE_W]) {
        _v3Velocity += _v3Forward * _fAcceleration * deltaTime;
    }
    else if (keystate[SDL_SCANCODE_S]) {
        _v3Velocity -= _v3Forward * _fAcceleration * deltaTime;
    }
    else {
        // Apply deceleration
        float decelerationFactor = pow(0.95f, deltaTime); // Adjust for frame rate independence
        _v3Velocity *= decelerationFactor;
    }

    // Clamp the velocity to the maximum speed
    _clampVelocity();

    if (keystate[SDL_SCANCODE_A]) {
        _fYaw -= TURN_SPEED;
    }
    if (keystate[SDL_SCANCODE_D]) {
        _fYaw += TURN_SPEED;
    }

    std::cout << "Velocity: X: " << _v3Velocity.x
        << " Y: " << _v3Velocity.y
        << " Z: " << _v3Velocity.z << std::endl;

    _v3Position += _v3Velocity;

    _setForward();
}

void BAE::PhysicsCharacter::_setForward()
{
    glm::vec3 front;
    front.x = cos(glm::radians(_fYaw)) * cos(glm::radians(_fPitch));
    front.y = sin(glm::radians(_fPitch));
    front.z = sin(glm::radians(_fYaw)) * cos(glm::radians(_fPitch));
    _v3Forward = glm::normalize(front);
}

void BAE::PhysicsCharacter::_clampVelocity()
{
    if (glm::length(_v3Velocity) > _fMaxSpeed) {
        _v3Velocity = glm::normalize(_v3Velocity) * _fMaxSpeed;
    }
}
