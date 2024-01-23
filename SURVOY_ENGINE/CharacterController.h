#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include <memory>
#include "PhysicsDebugDraw.h"
#include "Physics.h"

// https://github.com/222464/EvolvedVirtualCreaturesRepo/blob/master/VirtualCreatures/Volumetric_SDL/Source/SceneObjects/Physics/DynamicCharacterController.h

namespace BAE {
	class CharacterController
	{

	const float MOVEMENT_SPEED = 0.2f;

	public:
		CharacterController();
		~CharacterController();

		btRigidBody* PhysicsRef;
	};
}
