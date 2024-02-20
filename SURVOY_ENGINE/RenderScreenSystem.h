#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include "System.h"
#include "Shader.h"
#include "IObserver.h"
#include "EventManager.h"
#include "FrameBufferColourBufferEvent.h"
#include "Defaults.h"
#include "TextureComponent.h"
#include "EntityManager.h"

namespace ECS {
	class RenderScreenSystem : public System, IObserver
	{
    public:
        RenderScreenSystem(EntityManager& entityManager, EventManager& eventManager);
        ~RenderScreenSystem();

        void onNotify(const Event& event) override;

        void Load() override;
        void Render() override;

    private:
        std::vector<float> _quadVertices;
        unsigned int _VAO;
        unsigned int _VBO;
        unsigned int _colourBuffer;

        std::unique_ptr<ENGINE::Shader> _shader;

        EventManager& _eventManager;
        EntityManager& _entityManager;
	};
}
