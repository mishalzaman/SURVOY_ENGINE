#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <iostream>

#include "System.h"
#include "IObserver.h"
#include "Event.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "FontCharactersComponent.h"
#include "BuffersComponent.h"
#include "ProgramComponent.h"
#include "DebugPrintComponent.h"
#include "RenderTargetDimensionsComponent.h"

namespace ECS {
	class DebugScreenPrintSystem : public System, public IObserver
	{
	public:
		DebugScreenPrintSystem(EntityManager& entityManager, EventManager& eventManager);

		void onNotify(const Event& event) override;

		void Load() override;
		void Render() override;
		void UpdateOnFixedTimestep(float deltaTime) override;
		void Unload() override;

	private:
		EntityManager& _entityManager;
		EventManager& _eventManager;

		float _dt;

		void _renderLine(std::string text, float x, float y, glm::vec3 colour, float scale = 1);

		float _fontSize;
		float _lineHeight;
	};
}
