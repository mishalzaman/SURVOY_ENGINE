#pragma once

#include "Physics.h"

namespace ECS {
	struct GhostObjectCapsuleComponent
	{
	public:
		GhostObjectCapsuleComponent(float radius, float height) :
			GhostObject(nullptr), Radius(radius), Height(height) {}

		btPairCachingGhostObject* GhostObject;
		float Radius;
		float Height;
	};
}
