#pragma once

#include <GL/glew.h>
#include <iostream>
#include "Defaults.h"
#include "System.h"
#include "IObserver.h"
#include "EventManager.h"
#include "FrameBufferColourBufferEvent.h"

namespace ECS {
	class FBOEnabledSystem : public System, IObserver
	{
    public:
        FBOEnabledSystem(EventManager& eventManager);
        ~FBOEnabledSystem();

        void onNotify(const Event& event) override;

        void Load() override;
        void UpdatePrePhysics() override;
        void Update(float deltaTime) override;
        void UpdatePostPhysics() override;
        void Render() override;
        void Unload() override;

    private:
        EventManager& _eventManager;

        unsigned int _FBO;
        unsigned int _RBO;
        unsigned int _textureColorbuffer;
	};
}
