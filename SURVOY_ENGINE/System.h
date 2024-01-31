#pragma once

#include "EntityManager.h"  // Include the EntityManager definition
#include "Physics.h"

namespace ECS {
    class System {
    public:
        virtual ~System() = default; // Virtual destructor for proper cleanup of derived types

        // Update method signatures to accept a reference to EntityManager
        virtual void Load(EntityManager& entityManager, Physics& physics) = 0;
        virtual void Update(EntityManager& entityManager, Physics& physics) = 0;
        virtual void Update(float deltaTime, EntityManager& entityManager, Physics& physics) = 0;
        virtual void Renders(EntityManager& entityManager) = 0;
        virtual void Unload(EntityManager& entityManager, Physics& physics) = 0;
        virtual void UpdateVec3(EntityManager& entityManager, float x, float y, float z) = 0;

        // Add other common system functionality here if needed
    };
}
