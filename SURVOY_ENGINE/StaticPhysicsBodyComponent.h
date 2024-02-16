#pragma once

#include "Physics.h"

namespace ECS {
	struct StaticPhysicsBodyComponent
	{
		btRigidBody* Body;
	};
}
