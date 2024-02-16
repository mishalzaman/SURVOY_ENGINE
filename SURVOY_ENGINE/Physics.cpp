#include "Physics.h"
#include "Logger.h"

ENGINE::Physics::Physics()
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

	if (_world.get()) {
		_world->setGravity(btVector3(0, -9.8, 0));
	
		_world->setDebugDrawer(&_physicsDebugDraw);
		_world->getDebugDrawer()->setDebugMode(3);
	}
}

ENGINE::Physics::~Physics()
{
	for (int i = _world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
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

void ENGINE::Physics::DrawDebug()
{
	_physicsDebugDraw.SetMatrices(_view, _projection);
	_physicsDebugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	
	_world->debugDrawWorld();
}

void ENGINE::Physics::Simulate(float deltaTime)
{
	_world->stepSimulation(deltaTime);
}

void ENGINE::Physics::StaticTriangleMesh(const std::vector<SVertex>& vertices, glm::mat4 tranformation)
{
	btTriangleMesh* meshInterface = new btTriangleMesh;
	
	glm::mat4 transform = tranformation;
	
	for (int i = 0; i < vertices.size(); i += 3) {
		// Transform each vertex by the mesh's transformation matrix
		glm::vec4 transformedVertex0 = transform * glm::vec4(vertices[i].Position, 1.0f);
		glm::vec4 transformedVertex1 = transform * glm::vec4(vertices[i + 1].Position, 1.0f);
		glm::vec4 transformedVertex2 = transform * glm::vec4(vertices[i + 2].Position, 1.0f);
	
		btVector3 vertex0(transformedVertex0.x, transformedVertex0.y, transformedVertex0.z);
		btVector3 vertex1(transformedVertex1.x, transformedVertex1.y, transformedVertex1.z);
		btVector3 vertex2(transformedVertex2.x, transformedVertex2.y, transformedVertex2.z);
	
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

btDiscreteDynamicsWorld& ENGINE::Physics::World()
{
	if (!_world) {
		// Handle the error according to your application's needs
		throw std::runtime_error("Attempted to access a null world pointer in Physics::World()");
	}
	return *_world.get();
}
