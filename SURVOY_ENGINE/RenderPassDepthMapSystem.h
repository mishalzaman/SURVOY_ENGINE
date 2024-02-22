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
#include "NearFarPlanesComponent.h"
#include "ShadowResolutionComponent.h"

namespace ECS {
	class RenderPassDepthMapSystem : public System
	{
	public:
		RenderPassDepthMapSystem(EntityManager& entityManager);

		void Load() override;
		void Render() override;
		void Unload() override;
	private:
		EntityManager& _entityManager;
		std::vector<float> _quadVertices;
	};
}
