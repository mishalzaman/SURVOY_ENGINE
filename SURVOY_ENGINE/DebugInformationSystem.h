#pragma once

#include "System.h"
#include "EntityManager.h"
#include "Physics.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

namespace ECS {
	class DebugInformationSystem : public System
	{
	public:
		DebugInformationSystem(EntityManager& entityManager, Physics& physics);

		void Load() override;
		void UpdatePreFixedTimestep() override;
		void Render() override;
		void UpdateOnFixedTimestep(float deltaTime) override;
		void Unload() override;

	private:
		EntityManager& _entityManager;
		Physics& _physics;

		double _deltaTime;
	};
}
