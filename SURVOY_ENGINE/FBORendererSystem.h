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

namespace ECS {
	class FBORendererSystem : public System, IObserver
	{
    public:
        FBORendererSystem(EventManager& eventManager);
        ~FBORendererSystem();

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
	};
}
