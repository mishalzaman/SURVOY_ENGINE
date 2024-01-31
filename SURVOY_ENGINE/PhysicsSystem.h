#pragma once

#include "System.h"
#include "StaticPhysicsBodyComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"

namespace ECS {
    class PhysicsSystem : public System
    {
    public:
        PhysicsSystem();

        void Load(EntityManager& entityManager, Physics& physics) override;
        void Update(EntityManager& entityManager, Physics& physics) override;
        void Update(float deltaTime, EntityManager& entityManager, Physics& physics) override;
        void Renders(EntityManager& entityManager) override;
        void Unload(EntityManager& entityManager, Physics& physics) override;
        void UpdateVec3(EntityManager& entityManager, float x, float y, float z) override;

    private:

    };
}
