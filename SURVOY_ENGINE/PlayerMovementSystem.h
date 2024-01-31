#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "System.h"
#include "CharacterControllerComponent.h"
#include "DynamicPhysicsCapsuleComponent.h"

namespace ECS {
    class PlayerMovementSystem : public System
    {
    public:
        PlayerMovementSystem();

        void Load(EntityManager& entityManager, Physics& physics) override;
        void Update(EntityManager& entityManager, Physics& physics) override;
        void Update(float deltaTime, EntityManager& entityManager, Physics& physics) override;
        void Renders(EntityManager& entityManager) override;
        void Unload(EntityManager& entityManager, Physics& physics) override;
        void UpdateVec3(EntityManager& entityManager, float x, float y, float z) override;
    };
}
