#include "CameraSystem.h"

ECS::CameraSystem::CameraSystem(EntityManager& manager) : _entityManager(manager), _acceleration(2.f)
{
}

void ECS::CameraSystem::Load(std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>>& entities)
{
    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        // Retrieve the components required for rendering
        CameraComponent* camera = _entityManager.getComponent<CameraComponent>(entityId);

        if (camera) {
            _updateVectors(
                camera->Forward,
                camera->Up,
                camera->Right,
                camera->Yaw,
                camera->Pitch
            );

            camera->View = BAE::VectorHelpers::ViewMat4(camera->Position, camera->Forward, camera->Up);
            camera->Projection = BAE::VectorHelpers::ProjectionMat4(camera->ScreenWidth, camera->ScreenHeight, 60.0f);
        }
    }
}

void ECS::CameraSystem::Physics(float deltaTime, std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>>& entities)
{
    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        // Retrieve the components required for rendering
        CameraComponent* camera = _entityManager.getComponent<CameraComponent>(entityId);

        if (camera) {
            _move(deltaTime, camera->Position, camera->Forward, camera->Right);
            _mouseLook(deltaTime, camera->Yaw, camera->Pitch, camera->MouseX, camera->MouseY);

            _updateVectors(
                camera->Forward,
                camera->Up,
                camera->Right,
                camera->Yaw,
                camera->Pitch
            );

            camera->View = BAE::VectorHelpers::ViewMat4(camera->Position, camera->Forward, camera->Up);
            camera->Projection = BAE::VectorHelpers::ProjectionMat4(camera->ScreenWidth, camera->ScreenHeight, 60.0f);
        }
    }
}

void ECS::CameraSystem::Renders(std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>>& entities)
{
    // N/A
}

void ECS::CameraSystem::Unload(std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>>& entities)
{
    // N/A
}

void ECS::CameraSystem::_updateVectors(
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

void ECS::CameraSystem::_mouseLook(float deltaTime, float& yaw, float& pitch, float& mouseX, float& mouseY)
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

void ECS::CameraSystem::_move(float deltaTime, glm::vec3& position, const glm::vec3& forward, const glm::vec3& right)
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    float velocity = _acceleration * SPEED * deltaTime;

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
