#pragma once

#include "System.h"
#include "Physics.h"
#include "IObserver.h"
#include "EventManager.h"
#include "KeyPressEvent.h"

namespace ECS {
	class RenderPhysicsDebugSystem : public System, public IObserver
	{
	public:
		RenderPhysicsDebugSystem(Physics& physics, EventManager& eventManager);
		~RenderPhysicsDebugSystem();

		void onNotify(const Event& event) override;

		void Render() override;

	private:
		Physics& _phyiscs;
		EventManager& _eventManager;

		bool _show;
	};
}
