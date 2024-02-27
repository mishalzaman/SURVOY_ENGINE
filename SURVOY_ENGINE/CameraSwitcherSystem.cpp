#include "CameraSwitcherSystem.h"

ECS::CameraSwitcherSystem::CameraSwitcherSystem(EntityManager& entityManager, EventManager& eventManager) :
	_entityManager(entityManager), _eventManager(eventManager)
{
	_eventManager.subscribe(this);
}

ECS::CameraSwitcherSystem::~CameraSwitcherSystem()
{
	_eventManager.unsubscribe(this);
}

void ECS::CameraSwitcherSystem::onNotify(const Event& event)
{
    const auto* switchCamera = dynamic_cast<const SwitchCameraEvent*>(&event);

    if (switchCamera) {
		ECS::ActiveCameraComponent* activeCamera = _entityManager.getComponent<ECS::ActiveCameraComponent>(
			_entityManager.getIdByTag("ActiveCamera")
		);

		if (activeCamera->CameraTag == "CameraFirstPerson") {
			activeCamera->CameraTag = "CameraThirdPerson";
		}
		else {
			activeCamera->CameraTag = "CameraFirstPerson";
		}

		std::cout << activeCamera->CameraTag << std::endl;
    }
}
