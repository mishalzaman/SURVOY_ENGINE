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
        void UpdatePrePhysics() override;
        void Update(float deltaTime) override;
        void UpdatePostPhysics() override;
        void Render() override;
        void Unload() override;

    private:
        std::vector<float> _quadVertices;
        unsigned int _VAO;
        unsigned int _VBO;
        unsigned int _colourBuffer;

        std::unique_ptr<BAE::Shader> _shader;

        EventManager& _eventManager;
	};
}
