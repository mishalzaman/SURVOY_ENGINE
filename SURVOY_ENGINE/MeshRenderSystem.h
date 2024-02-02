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

namespace ECS {
    class MeshRenderSystem : public System, public IObserver {
    public:
        MeshRenderSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager);
        ~MeshRenderSystem();

        void onNotify(const Event& event) override;

        void Load() override;
        void Update() override;
        void Update(float deltaTime) override;
        void Renders() override;
        void Unload() override;

    private:
        std::unique_ptr<Shader> _defaultShader;
        glm::mat4 _view;
        glm::mat4 _projection;

        void _render(
            const TransformComponent& transform,
            const MeshComponent& mesh,
            const BuffersComponent& buffers,
            const TexturesComponent& textures
        );

        void _initBuffers(const MeshComponent& mesh, BuffersComponent& buffers);

        EntityManager& _entityManager;
        Physics& _physics;
        EventManager& _eventManager;
    };
}
