#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include <memory>
#include "PhysicsDebugDraw.h"
#include "Physics.h"
#include "VectorHelpers.h"

// https://github.com/222464/EvolvedVirtualCreaturesRepo/blob/master/VirtualCreatures/Volumetric_SDL/Source/SceneObjects/Physics/DynamicCharacterController.h

namespace BAE {
	class CharacterController
	{
		enum CharacterState {
			IDLE,
			WALKING,
			JUMPING
		};
	public:
		static constexpr float MOVEMENT_SPEED = 32.f;
		static constexpr float ACCELERATION = 4.0f;

		CharacterController(btDiscreteDynamicsWorld& world, btAlignedObjectArray<btCollisionShape*>& collisionShapes);
		~CharacterController();

		void CreatePhysicalCharacter(glm::vec3 position);

		void Move(float deltaTime);
		glm::vec3 Position();
		glm::vec3 Forward() { return _forward; }
		void UpdateYaw(float yaw);

	private:
		glm::vec3 _position;
		float _yaw;
		float _pitch;
		glm::vec3 _forward;
		glm::vec3 _right;
		glm::vec3 _up;

		btRigidBody* _physicalCharacter;
		CharacterState _state;

		void _updateVectors();
		bool _isOnGround();

		btDiscreteDynamicsWorld& _worldRef;
		btAlignedObjectArray<btCollisionShape*>& _collisionShapesRef;
	};
}

#endif