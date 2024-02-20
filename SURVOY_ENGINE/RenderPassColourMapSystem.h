#pragma once

#include <GL/glew.h>
#include <iostream>
#include "Defaults.h"
#include "System.h"
#include "IObserver.h"
#include "EventManager.h"
#include "FrameBufferColourBufferEvent.h"
#include "EntityManager.h"
#include "RenderPassComponent.h"
#include "BuffersComponent.h"
#include "TextureComponent.h"

namespace ECS {
	class RenderPassColourMapSystem : public System, IObserver
	{
    public:
        RenderPassColourMapSystem(EntityManager& entityManager, EventManager& eventManager);
        ~RenderPassColourMapSystem();

        void onNotify(const Event& event) override;

        void Load() override;
        void Render() override;
        void Unload() override;

    private:
        EventManager& _eventManager;
        EntityManager& _entityManager;

        unsigned int _FBO;
        unsigned int _RBO;
        unsigned int _textureColorbuffer;
	};
}
