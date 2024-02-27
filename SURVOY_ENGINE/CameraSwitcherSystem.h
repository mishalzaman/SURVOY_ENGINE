#pragma once

#include <string>
#include <iostream>

#include "System.h"
#include "IObserver.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "ActiveCameraComponent.h"
#include "SwitchCameraEvent.h"

namespace ECS {
	class CameraSwitcherSystem :public System, public IObserver
	{
	public:
		CameraSwitcherSystem(EntityManager& entityManager, EventManager& eventManager);
		~CameraSwitcherSystem();

		void onNotify(const Event& event) override;

	private:
		EntityManager& _entityManager;
		EventManager& _eventManager;

		std::string _currentCameraTag;
	};
}
