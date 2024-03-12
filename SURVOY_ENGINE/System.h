#pragma once
#include "ISystem.h"

namespace ECS {
	class System : public ISystem
	{
		virtual void Load() override {}
		virtual void UpdatePreFixedTimestep() override {}
        virtual void UpdateOnFixedTimestep(float deltaTime) override {}
        virtual void UpdateOnVariableTimestep() override {}
        virtual void Render() override {}
        virtual void Unload() override {}
	};
}
