#pragma once

#include "Physics.h"

namespace ECS {
	struct KinematicCapsulePhysicsBodyComponent
	{
	public:
		KinematicCapsulePhysicsBodyComponent(float radius, float height):
			Body(nullptr), Radius(radius), Height(height) {}

		btRigidBody* Body;
		float Radius;
		float Height;
	};
}
