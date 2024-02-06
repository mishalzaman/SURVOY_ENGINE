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
    const auto* inputEvent = dynamic_cast<const InputMouseRelXYEvent*>(&event);

    if (inputEvent) {
        std::vector<int> entities = _entityManager.getByTag("Camera Third Person");

        for (int entityId : entities) {
            ECS::CameraMouseComponent* mouse = _entityManager.getComponent<ECS::CameraMouseComponent>(entityId);

            if (mouse) {
                mouse->MouseRelX = inputEvent->getMouseX();
                mouse->MouseRelY = inputEvent->getMouseY();
            }
        }
    }
}

void ECS::CameraThirdPersonSystem::Load()
{
    std::vector<int> entities = _entityManager.getByTag("Camera Third Person");

    for (int entityId : entities) {
        ECS::ScreenDimensionsComponent* screen = _entityManager.getComponent<ECS::ScreenDimensionsComponent>(entityId);
        ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(entityId);
        ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

        if (screen && matrices && orientation) {
            matrices->View = BAE::VectorHelpers::ViewMat4(orientation->Position, orientation->Forward, orientation->Up);
            matrices->Projection = BAE::VectorHelpers::ProjectionMat4(screen->ScreenWidth, screen->ScreenHeight, 60.0f);

            _eventManager.notifyAll(CameraViewProjectionEvent(matrices->View, matrices->Projection));
            _eventManager.notifyAll(CameraPositionEvent(orientation->Position));

            _eventManager.notifyAll(CameraViewProjectionEvent(matrices->View, matrices->Projection));
            _eventManager.notifyAll(CameraPositionEvent(orientation->Position));
        }
    }
}

void ECS::CameraThirdPersonSystem::Update()
{
}

void ECS::CameraThirdPersonSystem::Update(float deltaTime)
{
    glm::vec3 target = glm::vec3(0);
    std::vector<int> entitiesPM = _entityManager.getByTag("Player Mesh");

    for (int entityId : entitiesPM) {
        ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);

        if (transform) {
            target = transform->Position;
        }
    }

    std::vector<int> entities = _entityManager.getByTag("Camera Third Person");

    for (int entityId : entities) {
        ECS::ScreenDimensionsComponent* screen = _entityManager.getComponent<ECS::ScreenDimensionsComponent>(entityId);
        ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(entityId);
        ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);
        ECS::CameraMouseComponent* mouse = _entityManager.getComponent<ECS::CameraMouseComponent>(entityId);

        if (screen && matrices && orientation && mouse) {
            // Process camera movement and orientation based on input and deltaTime
            _orbit(deltaTime, orientation->Yaw, orientation->Pitch, mouse->MouseRelX, mouse->MouseRelY, orientation->Position, orientation->Forward, orientation->Right, orientation->Up, target);

            // Calculate view and projection matrices
            matrices->View = BAE::VectorHelpers::ViewMat4(orientation->Position, orientation->Forward, orientation->Up);
            matrices->Projection = BAE::VectorHelpers::ProjectionMat4(screen->ScreenWidth, screen->ScreenHeight, 60.0f);

            _eventManager.notifyAll(CameraViewProjectionEvent(matrices->View, matrices->Projection));
            _eventManager.notifyAll(CameraPositionEvent(orientation->Position));
        }
    }
}

void ECS::CameraThirdPersonSystem::Renders()
{
}


void ECS::CameraThirdPersonSystem::Unload()
{
}

void ECS::CameraThirdPersonSystem::_orbit(float deltaTime, float& yaw, float& pitch, float& mouseX, float& mouseY, glm::vec3& position, glm::vec3& forward, glm::vec3& right, glm::vec3& up, glm::vec3 target)
{
    // Sensitivity scaling
    float sensitivity = MOUSE_SENSITIVITY * deltaTime;

    // Adjust yaw and pitch based on mouse movement
    yaw += mouseX * sensitivity;
    pitch += mouseY * sensitivity;

    // Clamp the pitch
    pitch = std::max(std::min(pitch, 89.0f), -89.0f);

    // Convert yaw and pitch to radians
    float yawRad = glm::radians(yaw);
    float pitchRad = glm::radians(pitch);

    // Recalculate the camera position
    glm::vec3 offset;
    offset.x = DISTANCE_TO_TARGET * cos(pitchRad) * cos(yawRad);
    offset.y = DISTANCE_TO_TARGET * sin(pitchRad);
    offset.z = DISTANCE_TO_TARGET * cos(pitchRad) * sin(yawRad);

    position = target - offset;

    // Update the camera's forward vector
    forward = glm::normalize(target - position);

    // Recalculate right and up vector
    right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));

    // Reset mouse offsets
    mouseX = 0;
    mouseY = 0;
}

void ECS::CameraThirdPersonSystem::_move(float deltaTime, glm::vec3& position, const glm::vec3& forward, const glm::vec3& right)
{
}
