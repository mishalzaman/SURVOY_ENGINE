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

void BAE::Physics::CreateLevelGeometry(const std::vector<SVertex>& vertices)
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

void BAE::Physics::UpdatePlayerGeometry(glm::vec3 velocity, glm::vec3 forward)
{
	// Convert glm::vec3 to btVector3 for Bullet Physics
	btVector3 bulletVelocity(velocity.x, velocity.y, velocity.z);

	_character->activate(true);

	// Set the linear velocity of the rigid body
	_character->setLinearVelocity(bulletVelocity * 40);

	// If you need to update the orientation as well
	// Compute the quaternion from the forward vector
	btVector3 forwardV(forward.x, forward.y, forward.z);
	forwardV.normalize();
	btVector3 up(0, 1, 0); // Assuming 'up' is in the Y direction
	btVector3 right = up.cross(forwardV).normalized();
	up = forwardV.cross(right).normalized();

	btMatrix3x3 orientation(right.x(), up.x(), forwardV.x(),
		right.y(), up.y(), forwardV.y(),
		right.z(), up.z(), forwardV.z());
	btQuaternion rotation;
	orientation.getRotation(rotation);

	// Set the rigid body's orientation
	btTransform transform = _character->getWorldTransform();
	transform.setRotation(rotation);
	_character->setWorldTransform(transform);
}

glm::vec3 BAE::Physics::PlayerPosition()
{
	btTransform trans;

	// position
	_character->getMotionState()->getWorldTransform(trans);
	btVector3 pos = trans.getOrigin(); // This is the position

	return glm::vec3(pos.x(), pos.y(), pos.z());
}

void BAE::Physics::CreatePlayerGeometry(glm::vec3 position, float yaw, float pitch)
{
	btCollisionShape* groundShape = new btCapsuleShape(0.5f, 1.f);
	_collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(position.x, position.y, position.z));

	// Create a quaternion from yaw and pitch
	btQuaternion rotation;
	rotation.setEuler(yaw, pitch, 0); // Assuming roll is zero
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

	_character = body;

	_world->addRigidBody(body);
}
