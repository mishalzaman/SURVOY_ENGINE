#include "CameraThirdPersonSystem.h"

ECS::CameraThirdPersonSystem::CameraThirdPersonSystem(
    EntityManager& entityManager,
    Physics& physics,
    EventManager& eventManager
) :
    _eventManager(eventManager),
    _entityManager(entityManager),
    _physics(physics)
{
	_eventManager.subscribe(this);
}

ECS::CameraThirdPersonSystem::~CameraThirdPersonSystem()
{
	_eventManager.unsubscribe(this);
}

void ECS::CameraThirdPersonSystem::onNotify(const Event& event)
{
}

void ECS::CameraThirdPersonSystem::Load()
{
    float yaw = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("CharacterController")
    )->Yaw; assert(yaw);

    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("Camera")
    ); assert(orientation);

    if (orientation) {
        orientation->Yaw = yaw;
        orientation->Pitch = 0.f;
    }
}

void ECS::CameraThirdPersonSystem::UpdateOnFixedTimestep(float deltaTime)
{
    ECS::OrientationComponent* characterOrientation = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("CharacterController")
    );

    int e = _entityManager.getIdByTag("Camera");

    ECS::RenderTargetDimensionsComponent* screen = _entityManager.getComponent<ECS::RenderTargetDimensionsComponent>(e);
    ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(e);
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::CameraMouseComponent* mouse = _entityManager.getComponent<ECS::CameraMouseComponent>(e);
    ECS::TargetComponent* target = _entityManager.getComponent<ECS::TargetComponent>(e);

    assert(screen);
    assert(matrices);
    assert(orientation);
    assert(mouse);
    assert(target);

    if (screen && matrices && orientation && mouse && target) {
        orientation->Yaw = characterOrientation->Yaw;

        // Process camera movement and orientation based on input and deltaTime
        _follow(
            deltaTime,
            orientation->Yaw,
            orientation->Pitch,
            mouse->MouseRelX,
            mouse->MouseRelY,
            orientation->Position,
            orientation->Forward,
            orientation->Right,
            orientation->Up,
            characterOrientation->Position
        );

        // Calculate view and projection matrices
        matrices->View = ENGINE::VectorHelpers::ViewMat4(orientation->Position, orientation->Forward, orientation->Up);
        matrices->Projection = ENGINE::VectorHelpers::ProjectionMat4(screen->ScreenWidth, screen->ScreenHeight, 60.0f);

        _eventManager.notifyAll(CameraViewProjectionEvent(matrices->View, matrices->Projection));
        _eventManager.notifyAll(CameraPositionEvent(orientation->Position));
        _eventManager.notifyAll(CameraYawEvent(orientation->Yaw));
    }
}

void ECS::CameraThirdPersonSystem::UpdateOnVariableTimestep()
{
}

void ECS::CameraThirdPersonSystem::_follow(float deltaTime, float& yaw, float& pitch, float& mouseX, float& mouseY, glm::vec3& position, glm::vec3& forward, glm::vec3& right, glm::vec3& up, glm::vec3 target)
{
    // Convert yaw and pitch to radians
    float yawRad = glm::radians(yaw);
    float pitchRad = glm::radians(pitch);

    // Recalculate the camera position
    glm::vec3 offset;
    offset.x = DISTANCE_TO_TARGET * cos(pitchRad) * cos(yawRad);
    offset.y = DISTANCE_TO_TARGET * sin(pitchRad);
    offset.z = DISTANCE_TO_TARGET * cos(pitchRad) * sin(yawRad);

    // set position based on character position
    position = target - offset;

    forward = glm::normalize(target - position);
    right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));

    // move camera right by offset
    glm::vec3 newTarget = target + right * RIGHT_OFFSET;
    position = (newTarget - offset);

    forward = glm::normalize(newTarget - position);

    // Move camera up by offset
    position += up * UP_OFFSET;
}
