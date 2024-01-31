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

namespace ECS {
    class MeshRenderSystem : public System {
    public:
        MeshRenderSystem();

        void Load(EntityManager& entityManager, Physics& physics) override;
        void Update(EntityManager& entityManager, Physics& physics) override;
        void Update(float deltaTime, EntityManager& entityManager, Physics& physics) override;
        void Renders(EntityManager& entityManager) override;
        void Unload(EntityManager& entityManager, Physics& physics) override;
        void UpdateVec3(EntityManager& entityManager, float x, float y, float z) override;

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
    };
}
