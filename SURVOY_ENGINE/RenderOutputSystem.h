#pragma once


#include "System.h"
#include "EntityManager.h"
#include "RenderPassComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "BuffersComponent.h"
#include "TexturesComponent.h"
#include "ProgramComponent.h"
#include "Defaults.h"
#include "DirectionalLightComponent.h"
#include "TextureComponent.h"
#include "LightSpaceMatrixComponent.h"
#include "CameraMatricesComponent.h"
#include "OrientationComponent.h"

namespace ECS {
    class RenderOutputSystem : public System
    {
    public:
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        float _nearPlane;
        float _farPlane;

        RenderOutputSystem(EntityManager& entityManager);

        void Render() override;

    private:
        EntityManager& _entityManager;

        void _renderForDepthMap();
        void _renderForColourMap();
    };
}
