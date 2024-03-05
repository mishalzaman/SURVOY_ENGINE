#pragma once

#include <glm/glm.hpp>
#include "System.h"
#include "StaticPhysicsBodyComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "EventManager.h"
#include "IObserver.h"
#include "CameraMatricesComponent.h"
#include "DynamicCapsulePhysicsBodyComponent.h"
#include "OrientationComponent.h"
#include "VelocityComponent.h"
#include "KinematicCapsulePhysicsBodyComponent.h"
#include "ActiveCameraComponent.h"
#include "GhostObjectCapsuleComponent.h"

namespace ECS {
    class PhysicsSystem : public System, public IObserver
    {
    public:
        PhysicsSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
        ~PhysicsSystem();

        void onNotify(const Event& event) override;

        void Load() override;
        void UpdateOnFixedTimestep(float deltaTime) override;

    private:
        EntityManager& _entityManager;
        Physics& _physics;
        EventManager& _eventManager;

        void _createStaticTriangleMeshBody();
        void _createDynamicCapsuleBody();
        void _createKinematiceCapsuleBody();
        void _createGhostObjectCapsule();
    };
}
