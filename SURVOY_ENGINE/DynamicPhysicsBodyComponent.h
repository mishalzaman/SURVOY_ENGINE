#pragma once

#include "Physics.h"

using namespace BAE;

namespace ECS {
	struct DynamicPhysicsBodyComponent
	{
		btRigidBody* Body;
		glm::vec3 velocity;
	};
}
