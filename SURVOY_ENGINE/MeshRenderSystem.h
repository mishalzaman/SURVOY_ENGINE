#pragma once

#include <iostream>
#include "System.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "BuffersComponent.h"
#include "TexturesComponent.h"
#include "CameraComponent.h"
#include "Shader.h"

namespace ECS {
	class MeshRenderSystem : public System
	{
	private:
		EntityManager& _entityManager;
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

	public:
		MeshRenderSystem(EntityManager& manager);

		void Load(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities) override;
		void Physics(float deltaTime, std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities) override;
		void Renders(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities);
		void Unload(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities);
	};
}
