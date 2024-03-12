#pragma once

// System
#include <memory>

// Engine
#include "EntityManager.h"
#include "ISystem.h"
#include "SystemManager.h"
#include "Shader.h"
#include "Physics.h"

// ECS - Components
#include "BuffersComponent.h"
#include "CameraMatricesComponent.h"
#include "CameraMouseComponent.h"
#include "DynamicCapsulePhysicsBodyComponent.h"
#include "MeshComponent.h"
#include "OrientationComponent.h"
#include "ProgramComponent.h"
#include "RenderTargetDimensionsComponent.h"
#include "StaticPhysicsBodyComponent.h"
#include "TexturesComponent.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "TargetComponent.h"
#include "SkyBoxComponent.h"
#include "TextureComponent.h"
#include "DirectionalLightComponent.h"
#include "LightSpaceMatrixComponent.h"
#include "CameraFOVComponent.h"
#include "NearFarPlanesComponent.h"
#include "ShadowResolutionComponent.h"
#include "FontCharactersComponent.h"
#include "DebugPrintComponent.h"
#include "KinematicCapsulePhysicsBodyComponent.h"
#include "ActiveCameraComponent.h"
#include "GhostObjectCapsuleComponent.h"
#include "MovementAttributesComponent.h"

// ECS - Systems
#include "CameraFreeLookSystem.h"
#include "CameraThirdPersonSystem.h"
#include "characterControllerSystem.h"
#include "PhysicsSystem.h"
#include "RenderStatic3DSystem.h"
#include "RenderPassColourMapSystem.h"
#include "RenderScreenSystem.h"
#include "SkyBoxSystem.h"
#include "RenderPhysicsDebugSystem.h"
#include "RenderPassDepthMapSystem.h"
#include "MeshStaticBuffersSystem.h"
#include "RenderOutputSystem.h"
#include "RenderDebugShadowMapSystem.h"
#include "DebugScreenPrintSystem.h"
#include "DebugGridSystem.h"
#include "CameraSwitcherSystem.h"
#include "PlayerInputSystem.h"
#include "PlayerPhysicsSystem.h"
#include "PlayerOutputSystem.h"
#include "DebugInformationSystem.h"

//  Observer
#include "Event.h"
#include "EventManager.h"
#include "IObserver.h"

#include <iostream>

class Scene
{
public:
    Scene();
    virtual ~Scene() = default;

    virtual void Load();
    virtual void UpdatePreFixedTimestep();
    virtual void UpdateOnFixedTimestep(float deltaTime);
    virtual void UpdateOnVariableTimestep();
    virtual void Render();
    virtual void Unload();

    EventManager& GetEventManager() { return *eventManager_; };

private:


protected:
    std::unique_ptr<ENGINE::Shader> defaultShader_;
    std::unique_ptr<ENGINE::Shader> skyboxShader_;
    std::unique_ptr<ENGINE::Shader> debugDepthQuadShader_;
    std::unique_ptr<ENGINE::Shader> depthShader_;
    std::unique_ptr<ENGINE::Shader> shadowMappedColourShader_;
    std::unique_ptr<ENGINE::Shader> fontShader_;
    std::unique_ptr<ENGINE::Physics> physics_;

    std::unique_ptr<ECS::SystemManager> systemManager_;
    std::unique_ptr<ECS::EntityManager> entityManager_;
    std::unique_ptr<EventManager> eventManager_;
};

