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
        RenderOutputSystem(EntityManager& entityManager);

        void Render() override;

    private:
        EntityManager& _entityManager;

        void _renderForDepthMap();
        void _renderForColourMap();
    };
}
