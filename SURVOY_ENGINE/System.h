#pragma once
#include "ISystem.h"

namespace ECS {
	class System : public ISystem
	{
		virtual void Load() override {}
		virtual void UpdatePrePhysics() override {}
        virtual void Update(float deltaTime) override {}
        virtual void UpdatePostPhysics() override {}
        virtual void Render() override {}
        virtual void Unload() override {}
	};
}
