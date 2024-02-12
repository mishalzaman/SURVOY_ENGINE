#pragma once

// System
#include <memory>

// Engine
#include "EntityManager.h"
#include "System.h"
#include "SystemManager.h"
#include "Shader.h"
#include "Physics.h"

// ECS - Components
#include "BuffersComponent.h"
#include "CameraComponent.h"
#include "CameraMatricesComponent.h"
#include "CameraMouseComponent.h"
#include "DynamicCapsulePhysicsBodyComponent.h"
#include "MeshComponent.h"
#include "OrientationComponent.h"
#include "ProgramComponent.h"
#include "ScreenDimensionsComponent.h"
#include "StaticPhysicsBodyComponent.h"
#include "TexturesComponent.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "TargetComponent.h"

// ECS - Systems
#include "CameraFreeLookSystem.h"
#include "CameraThirdPersonSystem.h"
#include "characterControllerSystem.h"
#include "PhysicsSystem.h"
#include "MeshRenderSystem.h"

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
    virtual void UpdatePrePhysics();
    virtual void UpdatePhysics(float deltaTime);
    virtual void UpdatePostPhysics();
    virtual void Render();
    virtual void Unload();

    EventManager& GetEventManager() { return *eventManager_; };

private:


protected:
    std::unique_ptr<BAE::Shader> defaultShader_;
    std::unique_ptr<BAE::Physics> physics_;

    std::unique_ptr<ECS::SystemManager> systemManager_;
    std::unique_ptr<ECS::EntityManager> entityManager_;
    std::unique_ptr<EventManager> eventManager_;
};

