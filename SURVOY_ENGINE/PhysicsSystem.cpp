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
    auto& entities = _entityManager.getEntityComponentIndices(); // Access the entity-component mapping

    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
        ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
        ECS::StaticPhysicsBodyComponent* staticPhysicsBody = _entityManager.getComponent<ECS::StaticPhysicsBodyComponent>(entityId);

        if (mesh && transform && staticPhysicsBody) {
			btTriangleMesh* meshInterface = new btTriangleMesh;

			glm::mat4 transformation = transform->transformation;

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

void ECS::PhysicsSystem::Update()
{
}

void ECS::PhysicsSystem::Update(float deltaTime)
{
}

void ECS::PhysicsSystem::Renders()
{
	_physics.DrawDebug();
}

void ECS::PhysicsSystem::Unload()
{
}
