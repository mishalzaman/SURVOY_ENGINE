#pragma once

#include <iostream>
#include <memory>
#include <typeindex>
#include <glm/glm.hpp>
#include "System.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "BuffersComponent.h"
#include "TexturesComponent.h"
#include "CameraComponent.h"
#include "Shader.h"
#include "Physics.h"
#include "IObserver.h"
#include "EventManager.h"
#include "CameraViewProjectionEvent.h"
#include "ProgramComponent.h"
#include "CameraMatricesComponent.h"
#include "CameraPositionEvent.h"
#include "OrientationComponent.h"

namespace ECS {
    class Renderer3DSystem : public System, public IObserver {
    public:
        Renderer3DSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
        ~Renderer3DSystem();

        void onNotify(const Event& event) override;

        void Load() override;
        void UpdatePrePhysics() override;
        void Update(float deltaTime) override;
        void UpdatePostPhysics() override;
        void Render() override;
        void Unload() override;

    private:
        void _render(
            const TransformComponent& transform,
            const MeshComponent& mesh,
            const BuffersComponent& buffers,
            const TexturesComponent& textures
        );

        EntityManager& _entityManager;
        Physics& _physics;
        EventManager& _eventManager;

        // Camera
        glm::vec3 _cameraPosition;
        glm::mat4 _projection;
        glm::mat4 _view;


    };
}
