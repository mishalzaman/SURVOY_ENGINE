#include "PhysicsSystem.h"

ECS::PhysicsSystem::PhysicsSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager):
	_entityManager(entityManager), _physics(physics), _eventManager(eventManager)
{
	_eventManager.subscribe(this);
}

ECS::PhysicsSystem::~PhysicsSystem()
{
	_eventManager.unsubscribe(this);
}

void ECS::PhysicsSystem::onNotify(const Event& event)
{
}

void ECS::PhysicsSystem::Load()
{
	_createStaticTriangleMeshBody();
	_createKinematiceCapsuleBody();
}

void ECS::PhysicsSystem::UpdateOnFixedTimestep(float deltaTime)
{
	std::string cameraString = _entityManager.getComponent<ECS::ActiveCameraComponent>(
		_entityManager.getIdByTag("ActiveCamera")
	)->CameraTag;

	CameraMatricesComponent* camera = _entityManager.getComponent<ECS::CameraMatricesComponent>(
		_entityManager.getIdByTag(cameraString)
	);

	_physics.View(camera->View);
	_physics.Projection(camera->Projection);
	_physics.Simulate(deltaTime);
}

void ECS::PhysicsSystem::_createStaticTriangleMeshBody()
{
	std::vector<int> entities = _entityManager.getByTag("Mesh");

	for (int entityId : entities) {
		ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
		ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
		ECS::StaticPhysicsBodyComponent* staticPhysicsBody = _entityManager.getComponent<ECS::StaticPhysicsBodyComponent>(entityId);

		if (mesh && transform && staticPhysicsBody) {
			btTriangleMesh* meshInterface = new btTriangleMesh;

			glm::mat4 transformation = transform->Transformation;

			for (int i = 0; i < mesh->Vertices.size(); i += 3) {
				glm::vec4 transformedVertex0 = transformation * glm::vec4(mesh->Vertices[i].Position, 1.0f);
				glm::vec4 transformedVertex1 = transformation * glm::vec4(mesh->Vertices[i + 1].Position, 1.0f);
				glm::vec4 transformedVertex2 = transformation * glm::vec4(mesh->Vertices[i + 2].Position, 1.0f);

				btVector3 vertex0(transformedVertex0.x, transformedVertex0.y, transformedVertex0.z);
				btVector3 vertex1(transformedVertex1.x, transformedVertex1.y, transformedVertex1.z);
				btVector3 vertex2(transformedVertex2.x, transformedVertex2.y, transformedVertex2.z);

				meshInterface->addTriangle(vertex0, vertex1, vertex2);
			}

			btBvhTriangleMeshShape* groundShape = new btBvhTriangleMeshShape(meshInterface, true, true);

			_physics.CollisionShapes().push_back(groundShape);

			btTransform groundTransform;
			groundTransform.setIdentity();

			btScalar mass(0.);

			btVector3 localInertia(0, 0, 0);
			if (mass != 0.f)
				groundShape->calculateLocalInertia(mass, localInertia);

			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			staticPhysicsBody->Body = body;

			int staticGroup = btBroadphaseProxy::StaticFilter;
			int staticCollidesWith = btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::StaticFilter; // Collide with everything except other static objects
			_physics.World().addRigidBody(body, staticGroup, staticCollidesWith);
		}
	}
}


void ECS::PhysicsSystem::_createDynamicCapsuleBody()
{
	std::vector<int> entitiesPC = _entityManager.getByTag("CharacterController");

	for (int entityId : entitiesPC) {
		ECS::DynamicCapsulePhysicsBodyComponent* dynamic = _entityManager.getComponent<ECS::DynamicCapsulePhysicsBodyComponent>(entityId);
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

		if (dynamic && orientation) {
			btCollisionShape* groundShape = new btCapsuleShape(0.25f, 1.25f);
			_physics.CollisionShapes().push_back(groundShape);

			btTransform groundTransform;
			groundTransform.setIdentity();
			groundTransform.setOrigin(btVector3(orientation->Position.x, orientation->Position.y, orientation->Position.z));

			// Create a quaternion from yaw and pitch
			btQuaternion rotation;
			rotation.setEuler(-90.f, 0.f, 0.f); // Assuming roll is zero
			groundTransform.setRotation(rotation);

			btScalar mass(1.f);
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				groundShape->calculateLocalInertia(mass, localInertia);

			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			// Set angular factor to zero to prevent rotation
			body->setAngularFactor(btVector3(0, 0, 0));

			dynamic->Body = body;

			_physics.World().addRigidBody(body);
		}
	}
}

void ECS::PhysicsSystem::_createKinematiceCapsuleBody()
{
	std::vector<int> entitiesPC = _entityManager.getByTag("CharacterController");

	std::cout << "Creating a kinematic body" << std::endl;

	for (int entityId : entitiesPC) {
		ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(entityId);
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

		if (kinematic && orientation) {
			btCollisionShape* groundShape = new btCapsuleShape(kinematic->Radius, kinematic->Height);
			_physics.CollisionShapes().push_back(groundShape);

			btTransform groundTransform;
			groundTransform.setIdentity();
			groundTransform.setOrigin(btVector3(orientation->Position.x, orientation->Position.y, orientation->Position.z));

			btQuaternion rotation;
			rotation.setEuler(-90.f, 0.f, 0.f); // Assuming roll is zero
			groundTransform.setRotation(rotation);

			btScalar mass(0.f); // Mass is 0 for kinematic bodies
			btVector3 localInertia(0, 0, 0);
			// No need to calculate inertia for kinematic bodies

			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			// Set the body to be kinematic
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			body->setActivationState(DISABLE_DEACTIVATION); // Prevent the body from being deactivated

			// Kinematic bodies don't need to set angular factor for preventing rotation, but you can still zero it if needed
			// body->setAngularFactor(btVector3(0, 0, 0));

			kinematic->Body = body;

			// Specify the collision group and mask for the kinematic body
			int kinematicCollisionGroup = btBroadphaseProxy::CharacterFilter;
			int kinematicCollisionMask = btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::StaticFilter; // Collide with everything except static objects

			_physics.World().addRigidBody(body, kinematicCollisionGroup, kinematicCollisionMask);
		}
	}
}
