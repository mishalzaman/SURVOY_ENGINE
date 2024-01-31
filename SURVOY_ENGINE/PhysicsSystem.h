#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include "System.h"
#include "StaticPhysicsBodyComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "DynamicPhysicsBodyComponent.h"

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
        void _static(EntityManager& entityManager, Physics& physics);
        void _dynamic(EntityManager& entityManager, Physics& physics);

        glm::vec3 computeAlignment(int entityId, EntityManager& entityManager);
        glm::vec3 computeCohesion(int entityId, EntityManager& entityManager);
        glm::vec3 computeSeparation(int entityId, EntityManager& entityManager);
    };
}
