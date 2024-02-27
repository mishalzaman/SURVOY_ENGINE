#include "RenderPhysicsDebugSystem.h"

ECS::RenderPhysicsDebugSystem::RenderPhysicsDebugSystem(Physics& physics, EventManager& eventManager):
	_phyiscs(physics), _eventManager(eventManager), _show(true)
{
	_eventManager.subscribe(this);
}

ECS::RenderPhysicsDebugSystem::~RenderPhysicsDebugSystem()
{
	_eventManager.unsubscribe(this);
}

void ECS::RenderPhysicsDebugSystem::onNotify(const Event& event)
{
	const auto* key = dynamic_cast<const KeyPressEvent*>(&event);

	if (key && key->getKey() == SDLK_z) {
		_show = !_show;
	}
}

void ECS::RenderPhysicsDebugSystem::Render()
{
	if (_show) { _phyiscs.DrawDebug(); }
}
