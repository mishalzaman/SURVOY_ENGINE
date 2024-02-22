#pragma once

#include <gl/glew.h>

#include "System.h"
#include "EntityManager.h"
#include "ProgramComponent.h"
#include "TextureComponent.h"
#include "BuffersComponent.h"
#include "NearFarPlanesComponent.h"
#include "Defaults.h"

namespace ECS {
	class RenderDebugShadowMapSystem :public System
	{
	public:
		RenderDebugShadowMapSystem(EntityManager& entityManager);

		void Render() override;

	private:
		EntityManager& _entityManager;
	};
}
