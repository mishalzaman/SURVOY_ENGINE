#include "PhysicsSystem.h"

ECS::PhysicsSystem::PhysicsSystem()
{
}

void ECS::PhysicsSystem::Load(EntityManager& entityManager, Physics& physics)
{
    auto& entities = entityManager.getEntityComponentIndices(); // Access the entity-component mapping

    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        ECS::MeshComponent* mesh = entityManager.getComponent<ECS::MeshComponent>(entityId);
        ECS::TransformComponent* transform = entityManager.getComponent<ECS::TransformComponent>(entityId);
        ECS::StaticPhysicsBodyComponent* staticPhysicsBody = entityManager.getComponent<ECS::StaticPhysicsBodyComponent>(entityId);

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

			physics.CollisionShapes().push_back(groundShape);

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
			physics.World().addRigidBody(body);
        }
    }
}

void ECS::PhysicsSystem::Update(EntityManager& entityManager, Physics& physics)
{
}

void ECS::PhysicsSystem::Update(float deltaTime, EntityManager& entityManager, Physics& physics)
{
}

void ECS::PhysicsSystem::Renders(EntityManager& entityManager)
{
}

void ECS::PhysicsSystem::Unload(EntityManager& entityManager, Physics& physics)
{
}

void ECS::PhysicsSystem::UpdateVec3(EntityManager& entityManager, float x, float y, float z)
{
}
