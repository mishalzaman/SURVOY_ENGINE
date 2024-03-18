#include "PlayerInputSystem.h"

ECS::PlayerInputSystem::PlayerInputSystem(EntityManager& entityManager):
	_entityManager(entityManager), _verticalVelocity(glm::vec3(0))
{
}

void ECS::PlayerInputSystem::UpdateOnFixedTimestep(float deltaTime)
{
    _turn(deltaTime);
    _move(deltaTime);
    //_applyGravity(deltaTime);
}

void ECS::PlayerInputSystem::_turn(float deltaTime)
{
    int e = _entityManager.getIdByTag("CharacterController");

    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);

    assert(orientation);
    assert(motion);

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_A]) {
        // Rotate left
        orientation->Yaw -= motion->TurnRate * deltaTime;
    }
    if (keystate[SDL_SCANCODE_D]) {
        // Rotate right
        orientation->Yaw += motion->TurnRate * deltaTime;
    }

    _updateVectors();
}

void ECS::PlayerInputSystem::_move(float deltaTime)
{
    int e = _entityManager.getIdByTag("CharacterController");

    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);

    assert(orientation);
    assert(motion);

    motion->Velocity = glm::vec3(0);

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    bool forward = keystate[SDL_SCANCODE_W];
    bool backward = keystate[SDL_SCANCODE_S];

    if (forward || backward) {
        if (forward) {
            motion->Velocity += orientation->Forward * motion->Acceleration * deltaTime;
        }
        if (backward) {
            motion->Velocity -= orientation->Forward * motion->Acceleration * deltaTime;
        }
    }
    else {
         //Normalize HorizontalVelocity if it's not a zero vector
        if (glm::length(motion->Velocity) > 0) {
            glm::vec3 velocityDirection = glm::normalize(motion->Velocity);
            // Decelerate in the direction opposite to the current velocity
            motion->Velocity -= velocityDirection * motion->Deceleration * deltaTime;

            // Clamp the velocity to 0 if the deceleration has reversed its direction
            if (glm::dot(velocityDirection, motion->Velocity) < 0) {
                motion->Velocity = glm::vec3(0, 0, 0);
            }
        }
    }

    // limit speed to max speed
    float velocity = glm::length(motion->Velocity);
    if (velocity > motion->Speed) {
        motion->Velocity = glm::normalize(motion->Velocity) * motion->Speed;
    }
}

void ECS::PlayerInputSystem::_applyGravity(float deltaTime)
{
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(motion);

    _verticalVelocity += motion->Gravity * deltaTime; // Gravity affects the y-component of velocity
    motion->Velocity = _verticalVelocity * deltaTime;
}

void ECS::PlayerInputSystem::_updateVectors()
{
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(orientation);

    orientation->Forward = ENGINE::VectorHelpers::ForwardVec3(orientation->Yaw, orientation->Pitch);
    orientation->Right = ENGINE::VectorHelpers::RightVec3(orientation->Forward);
    orientation->Up = ENGINE::VectorHelpers::UpVec3(orientation->Forward, orientation->Right);
}
