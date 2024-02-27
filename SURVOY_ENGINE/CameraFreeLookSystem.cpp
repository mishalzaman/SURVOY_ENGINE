#include "CameraFreeLookSystem.h"

ECS::CameraFreeLookSystem::CameraFreeLookSystem(
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

ECS::CameraFreeLookSystem::~CameraFreeLookSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::CameraFreeLookSystem::onNotify(const Event& event)
{
    const auto* inputEvent = dynamic_cast<const InputMouseRelXYEvent*>(&event);

    if (inputEvent) {
        std::vector<int> entities = _entityManager.getByTag("CameraFirstPerson");

        for (int entityId : entities) {
            ECS::CameraMouseComponent* mouse = _entityManager.getComponent<ECS::CameraMouseComponent>(entityId);

            if (mouse) {
                mouse->MouseRelX = inputEvent->getMouseX();
                mouse->MouseRelY = inputEvent->getMouseY();
            }
        }
    }
}

void ECS::CameraFreeLookSystem::Load()
{
    std::vector<int> entities = _entityManager.getByTag("CameraFirstPerson");

    for (int entityId : entities) {
        ECS::RenderTargetDimensionsComponent* screen = _entityManager.getComponent<ECS::RenderTargetDimensionsComponent>(entityId);
        ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(entityId);
        ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);
        ECS::CameraFOVComponent* fov = _entityManager.getComponent<ECS::CameraFOVComponent>(entityId);
        

        if (screen && matrices && orientation) {
            _updateVectors(
                orientation->Forward,
                orientation->Up,
                orientation->Right,
                orientation->Yaw,
                orientation->Pitch
            );

            matrices->View = ENGINE::VectorHelpers::ViewMat4(orientation->Position, orientation->Forward, orientation->Up);
            matrices->Projection = ENGINE::VectorHelpers::ProjectionMat4(screen->ScreenWidth, screen->ScreenHeight, fov->FOV);
        }
    }
}

void ECS::CameraFreeLookSystem::UpdateOnFixedTimestep(float deltaTime)
{
    if (_entityManager.getComponent<ECS::ActiveCameraComponent>(
        _entityManager.getIdByTag("ActiveCamera")
    )->CameraTag != "CameraFirstPerson") {
        return;
    }

    int e = _entityManager.getIdByTag("CameraFirstPerson");

    ECS::RenderTargetDimensionsComponent* screen = _entityManager.getComponent<ECS::RenderTargetDimensionsComponent>(e);
    ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(e);
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::CameraMouseComponent* mouse = _entityManager.getComponent<ECS::CameraMouseComponent>(e);
    ECS::CameraFOVComponent* fov = _entityManager.getComponent<ECS::CameraFOVComponent>(e);

    if (screen && matrices && orientation && mouse) {
        // Process camera movement and orientation based on input and deltaTime
        _mouseLook(deltaTime, orientation->Yaw, orientation->Pitch, mouse->MouseRelX, mouse->MouseRelY);
        _move(deltaTime, orientation->Position, orientation->Forward, orientation->Right);

        // Update the camera vectors and matrices
        _updateVectors(orientation->Forward, orientation->Up, orientation->Right, orientation->Yaw, orientation->Pitch);

        // Calculate view and projection matrices
        matrices->View = ENGINE::VectorHelpers::ViewMat4(orientation->Position, orientation->Forward, orientation->Up);
        matrices->Projection = ENGINE::VectorHelpers::ProjectionMat4(screen->ScreenWidth, screen->ScreenHeight, fov->FOV);
    }
    

    ECS::DebugPrintComponent* debugPrint = _entityManager.getComponent<ECS::DebugPrintComponent>(
        _entityManager.getIdByTag("DebugPrint")
    );

    if (debugPrint) {
        PrintText text1;
        text1.Text = "Camera Position X: " + std::to_string(orientation->Position.x) + " " +
            "Camera Y: " + std::to_string(orientation->Position.y) + " " +
            "Camera Z: " + std::to_string(orientation->Position.z);
        text1.Colour = glm::vec3(0, 1, 0);

        PrintText text2;
        text2.Text = "Camera Pitch: " + std::to_string(orientation->Pitch) + " " +
            "Camera Yaw: " + std::to_string(orientation->Yaw);
        text2.Colour = glm::vec3(0, 1, 0);
        
        debugPrint->PrintTexts.push_back(text1);
        debugPrint->PrintTexts.push_back(text2);
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

    right = ENGINE::VectorHelpers::RightVec3(forward);

    up = ENGINE::VectorHelpers::UpVec3(forward, right);
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

    if (keystate[SDL_SCANCODE_UP]) {
        position += forward * velocity;
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        position -= forward * velocity;
    }
    if (keystate[SDL_SCANCODE_LEFT]) {
        position -= right * velocity;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        position += right * velocity;
    }
}
