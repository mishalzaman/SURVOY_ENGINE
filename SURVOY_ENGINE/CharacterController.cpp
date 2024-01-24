#include "CharacterController.h"

BAE::CharacterController::CharacterController(btDiscreteDynamicsWorld& world, btAlignedObjectArray<btCollisionShape*>& collisionShapes):
	_position(glm::vec3(0)),
	_physicalCharacter(nullptr),
	_yaw(-90.0f),
	_pitch(0.0f),
	_state(CharacterState::IDLE),
	_worldRef(world),
	_collisionShapesRef(collisionShapes)
{
	_updateVectors();
}

BAE::CharacterController::~CharacterController()
{

}

void BAE::CharacterController::CreatePhysicalCharacter(
	glm::vec3 position
)
{
	_position = position;
	
	btCollisionShape* groundShape = new btCapsuleShape(0.5f, 1.f);
	_collisionShapesRef.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(_position.x, _position.y, _position.z));

	// Create a quaternion from yaw and pitch
	btQuaternion rotation;
	rotation.setEuler(_yaw, _pitch, 0); // Assuming roll is zero
	groundTransform.setRotation(rotation);

	btScalar mass(1.f);
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setAngularFactor(btVector3(0, 0, 0));

	_physicalCharacter = body;

	_worldRef.addRigidBody(body);
}

void BAE::CharacterController::Move(float deltaTime)
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	glm::vec3 direction(0.0f);

	if (keystate[SDL_SCANCODE_W]) {
		direction += _forward;
	}
	if (keystate[SDL_SCANCODE_S]) {
		direction -= _forward;
	}
	if (keystate[SDL_SCANCODE_A]) {
		direction -= _right;
	}
	if (keystate[SDL_SCANCODE_D]) {
		direction += _right;
	}

	if (glm::length(direction) > 0 && _physicalCharacter) {
		direction = glm::normalize(direction);
		float velocity = ACCELERATION * MOVEMENT_SPEED * deltaTime;

		// Set the velocity of the physical character
		_physicalCharacter->activate(true);
		_physicalCharacter->setLinearVelocity(btVector3(direction.x, direction.y, direction.z) * velocity);
	}
}

glm::vec3 BAE::CharacterController::Position()
{
	// Retrieve the updated position from Bullet's world transform
	btTransform trans;
	if (_physicalCharacter->getMotionState()) {
		_physicalCharacter->getMotionState()->getWorldTransform(trans);
		_position = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	}

	return _position;
}

void BAE::CharacterController::UpdateYaw(float yaw)
{
	_yaw = yaw;
	_updateVectors();
}

void BAE::CharacterController::_updateVectors()
{
	_forward = BAE::VectorHelpers::ForwardVec3(_yaw, _pitch);
	_right = BAE::VectorHelpers::RightVec3(_forward);
	_up = BAE::VectorHelpers::UpVec3(_forward, _right);
}

bool BAE::CharacterController::_isOnGround()
{
	const float CHECK_GROUND_DISTANCE = 1;

	btVector3 start = _physicalCharacter->getWorldTransform().getOrigin();
	btVector3 end = start - btVector3(0, CHECK_GROUND_DISTANCE, 0); // CHECK_GROUND_DISTANCE is the depth to check

	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);

	// Perform the ray test
	_worldRef.rayTest(start, end, rayCallback);

	return rayCallback.hasHit();
}
