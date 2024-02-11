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
	const auto* cameraEvent = dynamic_cast<const CameraViewProjectionEvent*>(&event);

	if (cameraEvent) {
		_physics.View(cameraEvent->getViewMatrix());
		_physics.Projection(cameraEvent->getProjectionMatrix());
	}
}

void ECS::PhysicsSystem::Load()
{
	_createStaticTriangleMeshBody();
	_createDynamicCapsuleBody();
}

void ECS::PhysicsSystem::UpdatePrePhysics()
{
}

void ECS::PhysicsSystem::Update(float deltaTime)
{
	_physics.Simulate(deltaTime);
}

void ECS::PhysicsSystem::UpdatePostPhysics()
{
}

void ECS::PhysicsSystem::Render()
{
	_physics.DrawDebug();
}

void ECS::PhysicsSystem::Unload()
{
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
				// Transform each vertex by the mesh's transformation matrix
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

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				groundShape->calculateLocalInertia(mass, localInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			staticPhysicsBody->Body = body;

			//add the body to the dynamics world
			_physics.World().addRigidBody(body);
		}
	}
}

void ECS::PhysicsSystem::_createDynamicCapsuleBody()
{
	std::vector<int> entitiesPC = _entityManager.getByTag("PlayerController");

	for (int entityId : entitiesPC) {
		ECS::DynamicCapsulePhysicsBodyComponent* dynamic = _entityManager.getComponent<ECS::DynamicCapsulePhysicsBodyComponent>(entityId);
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

		if (dynamic && orientation) {
			btCollisionShape* groundShape = new btCapsuleShape(0.25f, 1.25f);
			_physics.CollisionShapes().push_back(groundShape);

			btTransform groundTransform;
			groundTransform.setIdentity();
			groundTransform.setOrigin(btVector3(orientation->Position.x, orientation->Position.y + 1, orientation->Position.z));

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
