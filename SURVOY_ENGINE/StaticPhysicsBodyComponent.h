#pragma once

#include "Physics.h"

using namespace BAE;

namespace ECS {
	struct StaticPhysicsBodyComponent
	{
		btRigidBody* Body;
	};
}
