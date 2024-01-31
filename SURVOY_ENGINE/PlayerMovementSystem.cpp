#include "PlayerMovementSystem.h"

ECS::PlayerMovementSystem::PlayerMovementSystem()
{
}

void ECS::PlayerMovementSystem::Load(EntityManager& entityManager, Physics& physics)
{
    auto& entities = entityManager.getEntityComponentIndices(); // Access the entity-component mapping

    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        // Retrieve the CameraComponent required for the camera system
        ECS::CharacterControllerComponent* character = entityManager.getComponent<ECS::CharacterControllerComponent>(entityId);

        if (character) {
            // Extract position
            character->Position = glm::vec3(character->Transformation[3]);

            // Extract forward, right, and up vectors
            character->Right = glm::normalize(glm::vec3(character->Transformation[0]));  // X-axis
            character->Up = glm::normalize(glm::vec3(character->Transformation[1]));    // Y-axis
            character->Forward = -glm::normalize(glm::vec3(character->Transformation[2])); // Negative Z-axis

            // Calculate yaw and pitch
            // Yaw (rotation around y-axis) can be derived from the forward vector
            character->Yaw = glm::degrees(glm::atan(character->Forward.z, character->Forward.x));
            character->Pitch = 0;

            character->Direction = glm::vec3(0);
            character->Velocity = 0.f;
        }
    }
}

void ECS::PlayerMovementSystem::Update(EntityManager& entityManager, Physics& physics)
{
    auto& entities = entityManager.getEntityComponentIndices(); // Access the entity-component mapping

    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        // Retrieve the CameraComponent required for the camera system
        ECS::DynamicPhysicsCapsuleComponent* capsule = entityManager.getComponent<ECS::DynamicPhysicsCapsuleComponent>(entityId);
        ECS::CharacterControllerComponent* character = entityManager.getComponent<ECS::CharacterControllerComponent>(entityId);

        if (capsule && character) {
            capsule->Body->activate(true);
            capsule->Body->setLinearVelocity(btVector3(character->Direction.x, character->Direction.y, character->Direction.z) * character->Velocity);
        }
    }
}

void ECS::PlayerMovementSystem::Update(float deltaTime, EntityManager& entityManager, Physics& physics)
{
    auto& entities = entityManager.getEntityComponentIndices(); // Access the entity-component mapping

    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        // Retrieve the CameraComponent required for the camera system
        ECS::CharacterControllerComponent* character = entityManager.getComponent<ECS::CharacterControllerComponent>(entityId);

        if (character) {
            const Uint8* keystate = SDL_GetKeyboardState(NULL);

            character->Direction = glm::vec3(0.0f);

            if (keystate[SDL_SCANCODE_UP]) {
                character->Direction += character->Forward;
            }
            if (keystate[SDL_SCANCODE_DOWN]) {
                character->Direction -= character->Forward;
            }
            if (keystate[SDL_SCANCODE_LEFT]) {
                character->Direction -= character->Right;
            }
            if (keystate[SDL_SCANCODE_RIGHT]) {
                character->Direction += character->Right;
            }

            character->Direction = glm::normalize(character->Direction);
            character->Velocity = character->Acceleration * character->MovementSpeed * deltaTime;

            std::cout << character->Direction.x << std::endl;
        }
    }
}

void ECS::PlayerMovementSystem::Renders(EntityManager& entityManager)
{
}

void ECS::PlayerMovementSystem::Unload(EntityManager& entityManager, Physics& physics)
{
}

void ECS::PlayerMovementSystem::UpdateVec3(EntityManager& entityManager, float x, float y, float z)
{
}
