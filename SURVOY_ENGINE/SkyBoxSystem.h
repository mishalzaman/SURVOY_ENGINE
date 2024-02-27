#pragma once

#include <vector>
#include <iostream>
#include <gl/glew.h>
#include <typeindex>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "System.h"
#include "IObserver.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "SkyBoxComponent.h"
#include "STexture.h"
#include "FileLoader.h"
#include "ProgramComponent.h"
#include "ActiveCameraComponent.h"
#include "CameraMatricesComponent.h"

namespace ECS {
	class SkyBoxSystem :public System, IObserver
	{
	public:
		SkyBoxSystem(EntityManager& entityManager, EventManager& _eventManager);
		~SkyBoxSystem();

		void onNotify(const Event& event) override;

		void Load() override;
		void Render() override;
		void Unload() override;

	private:
		EntityManager& _entityManager;
		EventManager& _eventManager;

		unsigned int _VAO;
		unsigned int _VBO;
		unsigned int _texture;

		std::vector<float> _cube;

		glm::mat4 _projection;
		glm::mat4 _view;
	};
}
