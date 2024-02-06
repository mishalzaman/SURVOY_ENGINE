#pragma once

// Base
#include "SystemManager.h"
#include "System.h"
#include "EntityManager.h"

// Components
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

// Systems
#include "CameraFreeLookSystem.h"
#include "CameraThirdPersonSystem.h"
#include "characterControllerSystem.h"
#include "PhysicsSystem.h"
#include "MeshRenderSystem.h"

// Events
#include "CameraPositionEvent.h"
#include "CameraViewProjectionEvent.h"
#include "InputMouseRelXYEvent.h"

//  Observer
#include "Event.h"
#include "EventManager.h"
#include "IObserver.h"

// Others
#include "Physics.h"