#pragma once

#include "Physics.h"

using namespace BAE;

namespace ECS {
	struct DynamicPhysicsCapsuleComponent
	{
		btRigidBody* Body;
	};
}
