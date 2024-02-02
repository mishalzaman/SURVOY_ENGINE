#pragma once

#include "System.h"
#include "StaticPhysicsBodyComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"

namespace ECS {
    class PhysicsSystem : public System
    {
    public:
        PhysicsSystem(EntityManager& entityManager, Physics& physics);

        void Load() override;
        void Update() override;
        void Update(float deltaTime) override;
        void Renders() override;
        void Unload() override;

    private:
        EntityManager& _entityManager;
        Physics& _physics;
    };
}
