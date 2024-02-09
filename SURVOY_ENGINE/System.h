#pragma once

#include "EntityManager.h"  // Include the EntityManager definition
#include "Physics.h"

namespace ECS {
    class System {
    public:
        virtual ~System() = default; // Virtual destructor for proper cleanup of derived types

        // Update method signatures to accept a reference to EntityManager
        virtual void Load() = 0;
        virtual void UpdatePrePhysics() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Render() = 0;
        virtual void Unload() = 0;

        // Add other common system functionality here if needed
    };
}
