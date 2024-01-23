#include "CharacterController.h"

BAE::CharacterController::CharacterController():
	_position(glm::vec3(0)),
	_physicalCharacter(nullptr),
	_yaw(-90.0f),
	_pitch(0.0f)
{
}

BAE::CharacterController::~CharacterController()
{

}

void BAE::CharacterController::CreatePhysicalCharacter(
	glm::vec3 position,
	btDiscreteDynamicsWorld* world,
	btAlignedObjectArray<btCollisionShape*> collisionShapes
)
{
	_position = position;
	
	btCollisionShape* groundShape = new btCapsuleShape(0.5f, 1.f);
	collisionShapes.push_back(groundShape);

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

	world->addRigidBody(body);
}
