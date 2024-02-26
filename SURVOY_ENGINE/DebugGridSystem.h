#pragma once

#include <memory>

#include "System.h"
#include "EntityManager.h"
#include "Grid.h"
#include "CameraMatricesComponent.h"
#include "OrientationComponent.h"

namespace ECS {
	class DebugGridSystem :public System
	{
	public:
		DebugGridSystem(EntityManager& entitySystem);

		void Load() override;
		void Render() override;

	private:
		EntityManager& _entityManager;

		std::unique_ptr<ENGINE::Grid> _grid;
	};

}
