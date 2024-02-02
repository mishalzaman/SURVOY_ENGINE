#include "CameraFreeLookSystem.h"

ECS::CameraFreeLookSystem::CameraFreeLookSystem(
    EntityManager& entityManager,
    Physics& physics,
    EventManager& eventManager,
    int cameraEntityId
    ) :
    _eventManager(eventManager),
    _cameraEntityId(cameraEntityId),
    _entityManager(entityManager),
    _physics(physics)
{
    _eventManager.subscribe(this);
}

ECS::CameraFreeLookSystem::~CameraFreeLookSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::CameraFreeLookSystem::onNotify(const Event& event)
{
    const auto* inputEvent = dynamic_cast<const InputMouseRelXYEvent*>(&event);

    if (inputEvent) {
        ECS::CameraMouseComponent* mouse = _entityManager.getComponent<ECS::CameraMouseComponent>(_cameraEntityId);

        if (mouse) {
            mouse->MouseRelX = inputEvent->getMouseX();
            mouse->MouseRelY = inputEvent->getMouseY();
        }
    }
}

void ECS::CameraFreeLookSystem::Load()
{
    ECS::ScreenDimensionsComponent* screen = _entityManager.getComponent<ECS::ScreenDimensionsComponent>(_cameraEntityId);
    ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(_cameraEntityId);
    ECS::CameraOrientationComponent* orientation = _entityManager.getComponent<ECS::CameraOrientationComponent>(_cameraEntityId);

    if (screen && matrices && orientation) {
        _updateVectors(
            orientation->Forward,
            orientation->Up,
            orientation->Right,
            orientation->Yaw,
            orientation->Pitch
        );

        matrices->View = BAE::VectorHelpers::ViewMat4(orientation->Position, orientation->Forward, orientation->Up);
        matrices->Projection = BAE::VectorHelpers::ProjectionMat4(screen->ScreenWidth, screen->ScreenHeight, 60.0f);

        _eventManager.notifyAll(CameraViewProjectionEvent(matrices->View, matrices->Projection));
    }
}

void ECS::CameraFreeLookSystem::Renders()
{
    // N/A
}

void ECS::CameraFreeLookSystem::Unload()
{
    // N/A
}

void ECS::CameraFreeLookSystem::Update()
{
    // N/A
}

void ECS::CameraFreeLookSystem::Update(float deltaTime)
{
    ECS::ScreenDimensionsComponent* screen = _entityManager.getComponent<ECS::ScreenDimensionsComponent>(_cameraEntityId);
    ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(_cameraEntityId);
    ECS::CameraOrientationComponent* orientation = _entityManager.getComponent<ECS::CameraOrientationComponent>(_cameraEntityId);
    ECS::CameraMouseComponent* mouse = _entityManager.getComponent<ECS::CameraMouseComponent>(_cameraEntityId);

    if (screen && matrices && orientation && mouse) {
        // Process camera movement and orientation based on input and deltaTime
        _mouseLook(deltaTime, orientation->Yaw, orientation->Pitch, mouse->MouseRelX, mouse->MouseRelY);
        _move(deltaTime, orientation->Position, orientation->Forward, orientation->Right);

        // Update the camera vectors and matrices
        _updateVectors(orientation->Forward, orientation->Up, orientation->Right, orientation->Yaw, orientation->Pitch);

        // Calculate view and projection matrices
        matrices->View = BAE::VectorHelpers::ViewMat4(orientation->Position, orientation->Forward, orientation->Up);
        matrices->Projection = BAE::VectorHelpers::ProjectionMat4(screen->ScreenWidth, screen->ScreenHeight, 60.0f);

        _eventManager.notifyAll(CameraViewProjectionEvent(matrices->View, matrices->Projection));
    }
}

void ECS::CameraFreeLookSystem::_updateVectors(
    glm::vec3& forward,
    glm::vec3& up,
    glm::vec3& right,
    float& yaw,
    float& pitch
)
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    forward = glm::normalize(front);

    right = BAE::VectorHelpers::RightVec3(forward);

    up = BAE::VectorHelpers::UpVec3(forward, right);
}

void ECS::CameraFreeLookSystem::_mouseLook(float deltaTime, float& yaw, float& pitch, float& mouseX, float& mouseY)
{
    float xOffset = mouseX;
    float yOffset = mouseY;

    xOffset *= MOUSE_SENSITIVITY * deltaTime;
    yOffset *= MOUSE_SENSITIVITY * deltaTime;

    yaw += xOffset;
    pitch -= yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    mouseX = 0;
    mouseY = 0;
}

void ECS::CameraFreeLookSystem::_move(float deltaTime, glm::vec3& position, const glm::vec3& forward, const glm::vec3& right)
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    float velocity = ACCELERATION * SPEED * deltaTime;

    if (keystate[SDL_SCANCODE_W]) {
        position += forward * velocity;
    }
    if (keystate[SDL_SCANCODE_S]) {
        position -= forward * velocity;
    }
    if (keystate[SDL_SCANCODE_A]) {
        position -= right * velocity;
    }
    if (keystate[SDL_SCANCODE_D]) {
        position += right * velocity;
    }
}
