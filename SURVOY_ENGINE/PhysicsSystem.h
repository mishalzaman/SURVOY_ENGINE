#pragma once

#include <glm/glm.hpp>
#include "System.h"
#include "StaticPhysicsBodyComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "EventManager.h"
#include "IObserver.h"
#include "CameraViewProjectionEvent.h"
#include "CameraMatricesComponent.h"

namespace ECS {
    class PhysicsSystem : public System, public IObserver
    {
    public:
        PhysicsSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
        ~PhysicsSystem();

        void onNotify(const Event& event) override;

        void Load() override;
        void Update() override;
        void Update(float deltaTime) override;
        void Renders() override;
        void Unload() override;

    private:
        EntityManager& _entityManager;
        Physics& _physics;
        EventManager& _eventManager;

        glm::mat4 _view;
        glm::mat4 _projection;
    };
}
