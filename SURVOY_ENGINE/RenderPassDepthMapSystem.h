#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

#include "System.h"
#include "EntityManager.h"
#include "BuffersComponent.h"
#include "TextureComponent.h"
#include "DirectionalLightComponent.h"
#include "ProgramComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "BuffersComponent.h"
#include "TexturesComponent.h"
#include "Defaults.h"
#include "LightSpaceMatrixComponent.h"
#include "CameraMatricesComponent.h"
#include "RenderPassComponent.h"

namespace ECS {
	class RenderPassDepthMapSystem : public System
	{
	public:
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		RenderPassDepthMapSystem(EntityManager& entityManager);

		void Load() override;
		void Render() override;
		void Unload() override;
	private:
		float _nearPlane;
		float _farPlane;

		EntityManager& _entityManager;
		std::vector<float> _quadVertices;

		void _renderMeshes(
			const TransformComponent& transform,
			const MeshComponent& mesh,
			const BuffersComponent& buffers,
			const TexturesComponent& textures
		);

		void _renderDepthDebugQuad();
	};
}
