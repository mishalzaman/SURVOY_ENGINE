#include "Physics.h"

BAE::Physics::Physics()
{
	_initialize();

	if (_world.get()) {
		_world->setGravity(btVector3(0, -10, 0));

		_world->setDebugDrawer(&_physicsDebugDraw);
		_world->getDebugDrawer()->setDebugMode(3);
	}
}

BAE::Physics::~Physics() {
	for (int i = _world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();

			if (_character == body) {
				_character = nullptr;  // Set to nullptr if it's the same as the deleted body
			}
		}
		_world->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < _collisionShapes.size(); j++)
	{
		btCollisionShape* shape = _collisionShapes[j];
		_collisionShapes[j] = 0;
		delete shape;
	}
}

void BAE::Physics::Simulate(float deltaTime)
{
    _world->stepSimulation(deltaTime);
}

void BAE::Physics::DrawDebug(glm::mat4 projection, glm::mat4 view)
{
    _physicsDebugDraw.SetMatrices(view, projection);
	_physicsDebugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);

    _world->debugDrawWorld();
}

void BAE::Physics::CreateStaticShape(const std::vector<SVertex>& vertices)
{
	btTriangleMesh* meshInterface = new btTriangleMesh;

	for (int i = 0; i < vertices.size(); i += 3) {
		btVector3 vertex0(vertices[i].Position.x, vertices[i].Position.y, vertices[i].Position.z);
		btVector3 vertex1(vertices[i + 1].Position.x, vertices[i + 1].Position.y, vertices[i + 1].Position.z);
		btVector3 vertex2(vertices[i + 2].Position.x, vertices[i + 2].Position.y, vertices[i + 2].Position.z);

		meshInterface->addTriangle(vertex0, vertex1, vertex2);
	}

	btBvhTriangleMeshShape* groundShape = new btBvhTriangleMeshShape(meshInterface, true, true);

	_collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();

	btScalar mass(0.);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	_world->addRigidBody(body);
}

void BAE::Physics::_initialize()
{
	// create the collision configuration
	_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();

	// create the dispatcher
	_dispatcher = std::make_unique<btCollisionDispatcher>(_collisionConfiguration.get());

	// create the broadphase
	_broadphase = std::make_unique<btDbvtBroadphase>();

	// create the constraint solver
	_solver = std::make_unique<btSequentialImpulseConstraintSolver>();

	// create the world
	_world = std::make_unique<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadphase.get(), _solver.get(), _collisionConfiguration.get());
}

void BAE::Physics::CreateCapsule(glm::vec3 position)
{
	btCollisionShape* groundShape = new btCapsuleShape(0.5f, 1.f);

	_collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(position.x, position.y, position.z));

	btScalar mass(100.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setAngularFactor(btVector3(0, 1, 0));

	_character = body;

	//add the body to the dynamics world
	_world->addRigidBody(body);
}

void BAE::Physics::IterateShapes(glm::vec3 forward, glm::vec3 velocity, float yaw)
{
	for (int j = _world->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = _world->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
		{
			if (body->getCollisionShape()->getName() == "CapsuleShape")
			{
				// Activate the body
				body->activate(true);

				// Calculate the forward direction based on yaw
				// Assuming yaw is in radians and affects the Y-axis
				btVector3 forwardDirection(forward.x, forward.y, forward.z);
				forwardDirection.normalize();

				// Apply the calculated direction to the velocity
				btVector3 newVelocity = forwardDirection * velocity.length();
				body->setLinearVelocity(newVelocity);
			}
		}
	}
}

void BAE::Physics::UpdateCharacter(glm::vec3 forward, glm::vec3 velocity)
{
	btTransform capsuleTransform;
	_character->getMotionState()->getWorldTransform(capsuleTransform);

	// Activate the body
	_character->activate(true);

	// Calculate the forward direction based on yaw
	// Assuming yaw is in radians and affects the Y-axis
	btVector3 forwardDirection(forward.x, forward.y, forward.z);
	forwardDirection.normalize();

	// Apply the calculated direction to the velocity
	btVector3 newVelocity = forwardDirection * velocity.length();
	_character->setLinearVelocity(newVelocity);
}

glm::vec3 BAE::Physics::GetCharacterOrigin()
{
	if (_character) {
		const btTransform& trans = _character->getWorldTransform();
		btVector3 pos = trans.getOrigin();
		return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
	}
	return glm::vec3();  // Return a default vector if _character is nullptr
}

float BAE::Physics::GetCharacterYaw()
{
	if (_character) {
		const btTransform& trans = _character->getWorldTransform();
		btQuaternion rot = trans.getRotation();
		btScalar roll, pitch, yaw;
		trans.getBasis().getEulerYPR(yaw, pitch, roll);
		return yaw;
	}
	return 0;
}
