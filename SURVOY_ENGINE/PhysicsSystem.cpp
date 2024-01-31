#include "PhysicsSystem.h"
#include <glm/gtx/quaternion.hpp>

ECS::PhysicsSystem::PhysicsSystem()
{
}

void ECS::PhysicsSystem::Load(EntityManager& entityManager, Physics& physics)
{
	_static(entityManager, physics);
	_dynamic(entityManager, physics);
}

void ECS::PhysicsSystem::Update(EntityManager& entityManager, Physics& physics)
{
}

void ECS::PhysicsSystem::Update(float deltaTime, EntityManager& entityManager, Physics& physics)
{
	auto& entities = entityManager.getEntityComponentIndices();

	float MAX_VELOCITY = 10.f;

	for (const auto& entityPair : entities) {
		int entityId = entityPair.first;

		ECS::DynamicPhysicsBodyComponent* dynamicBody = entityManager.getComponent<ECS::DynamicPhysicsBodyComponent>(entityId);
		ECS::TransformComponent* transform = entityManager.getComponent<ECS::TransformComponent>(entityId);

		if (dynamicBody && transform) {
			// Compute flocking behavior
			glm::vec3 alignment = computeAlignment(entityId, entityManager);
			glm::vec3 cohesion = computeCohesion(entityId, entityManager);
			glm::vec3 separation = computeSeparation(entityId, entityManager);

			glm::vec3 flockingForce = alignment + cohesion + separation;

			// Update the velocity based on the flocking force
			dynamicBody->velocity += flockingForce * deltaTime;

			// Limit the velocity to a maximum value if necessary
			float maxVelocity = MAX_VELOCITY; // Define MAX_VELOCITY as per your requirements
			if (glm::length(dynamicBody->velocity) > maxVelocity) {
				dynamicBody->velocity = glm::normalize(dynamicBody->velocity) * maxVelocity;
			}

			// Update the body's velocity in the physics world
			btRigidBody* body = dynamicBody->Body;
			if (body) {
				body->setLinearVelocity(btVector3(dynamicBody->velocity.x, 0, dynamicBody->velocity.z));

				// Update the transform based on the physics simulation
				btTransform btTrans;
				body->getMotionState()->getWorldTransform(btTrans);
				// Extract the new position from btTrans
				glm::vec3 newPosition(btTrans.getOrigin().getX(), btTrans.getOrigin().getY(), btTrans.getOrigin().getZ());

				// Decompose the original transformation matrix
				glm::vec3 scale, translation, skew;
				glm::vec4 perspective;
				glm::quat rotation;
				glm::decompose(transform->transformation, scale, rotation, translation, skew, perspective);

				// Create a new translation matrix with the updated position
				glm::mat4 newTranslationMatrix = glm::translate(glm::mat4(1.0f), newPosition);

				// Create a rotation matrix from the quaternion
				glm::mat4 rotationMatrix = glm::toMat4(rotation);

				// Create a scale matrix
				glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

				// Combine the matrices to form the updated transformation matrix
				transform->transformation = newTranslationMatrix * rotationMatrix * scaleMatrix;
			}
		}
	}
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

void ECS::PhysicsSystem::_static(EntityManager& entityManager, Physics& physics)
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

void ECS::PhysicsSystem::_dynamic(EntityManager& entityManager, Physics& physics)
{
	auto& entities = entityManager.getEntityComponentIndices(); // Access the entity-component mapping

	for (const auto& entityPair : entities) {
		int entityId = entityPair.first;

		ECS::MeshComponent* mesh = entityManager.getComponent<ECS::MeshComponent>(entityId);
		ECS::TransformComponent* transform = entityManager.getComponent<ECS::TransformComponent>(entityId);
		ECS::DynamicPhysicsBodyComponent* dynamicPhysicsBody = entityManager.getComponent<ECS::DynamicPhysicsBodyComponent>(entityId);

		if (mesh && transform && dynamicPhysicsBody) {
			glm::vec3 position = transform->position;

			btCollisionShape* groundShape = new btSphereShape(btScalar(0.5f));

			physics.CollisionShapes().push_back(groundShape);

			btTransform groundTransform;
			groundTransform.setIdentity();
			groundTransform.setOrigin(btVector3(position.x, position.y, position.z));

			// Create a quaternion from yaw and pitch
			btQuaternion rotation;
			rotation.setEuler(-90.f, 0, 0); // Assuming roll is zero
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

			dynamicPhysicsBody->Body = body;

			physics.World().addRigidBody(body);
		}
	}
}

glm::vec3 ECS::PhysicsSystem::computeAlignment(int entityId, EntityManager& entityManager) {
	glm::vec3 averageVelocity(0.0f);
	int neighborCount = 0;
	float ALIGNMENT_RADIUS = 2.f;

	// Loop through entities to find neighbors
	auto& entities = entityManager.getEntityComponentIndices();
	for (const auto& entityPair : entities) {
		int otherEntityId = entityPair.first;
		if (otherEntityId != entityId) {
			ECS::TransformComponent* otherTransform = entityManager.getComponent<ECS::TransformComponent>(otherEntityId);
			ECS::DynamicPhysicsBodyComponent* otherBody = entityManager.getComponent<ECS::DynamicPhysicsBodyComponent>(otherEntityId);

			if (otherTransform && otherBody) {
				// Check if the other entity is within a certain radius
				float distance = glm::distance(otherTransform->position, entityManager.getComponent<ECS::TransformComponent>(entityId)->position);
				if (distance < ALIGNMENT_RADIUS) {
					averageVelocity += otherBody->velocity;
					neighborCount++;
				}
			}
		}
	}

	if (neighborCount > 0) {
		averageVelocity /= static_cast<float>(neighborCount);
		// Steer towards this average velocity
	}

	return averageVelocity;
}


glm::vec3 ECS::PhysicsSystem::computeCohesion(int entityId, EntityManager& entityManager) {
	glm::vec3 centerOfMass(0.0f);
	int neighborCount = 0;
	float COHESION_RADIUS = 10.f;

	// Loop through entities to find neighbors
	auto& entities = entityManager.getEntityComponentIndices();
	for (const auto& entityPair : entities) {
		int otherEntityId = entityPair.first;
		if (otherEntityId != entityId) {
			ECS::TransformComponent* otherTransform = entityManager.getComponent<ECS::TransformComponent>(otherEntityId);

			if (otherTransform) {
				// Check if the other entity is within a certain radius
				float distance = glm::distance(otherTransform->position, entityManager.getComponent<ECS::TransformComponent>(entityId)->position);
				if (distance < COHESION_RADIUS) {
					centerOfMass += otherTransform->position;
					neighborCount++;
				}
			}
		}
	}

	if (neighborCount > 0) {
		centerOfMass /= static_cast<float>(neighborCount);
		// Steer towards this center of mass
	}

	return centerOfMass;
}

glm::vec3 ECS::PhysicsSystem::computeSeparation(int entityId, EntityManager& entityManager) {
	glm::vec3 avoidVector(0.0f);
	int neighborCount = 0;
	float SEPARATION_RADIUS = 4.f;

	// Loop through entities to find neighbors
	auto& entities = entityManager.getEntityComponentIndices();
	for (const auto& entityPair : entities) {
		int otherEntityId = entityPair.first;
		if (otherEntityId != entityId) {
			ECS::TransformComponent* otherTransform = entityManager.getComponent<ECS::TransformComponent>(otherEntityId);

			if (otherTransform) {
				// Check if the other entity is within a certain radius
				float distance = glm::distance(otherTransform->position, entityManager.getComponent<ECS::TransformComponent>(entityId)->position);
				if (distance < SEPARATION_RADIUS && distance > 0.0f) {
					glm::vec3 difference = entityManager.getComponent<ECS::TransformComponent>(entityId)->position - otherTransform->position;
					difference /= distance; // Weight by distance
					avoidVector += difference;
					neighborCount++;
				}
			}
		}
	}

	if (neighborCount > 0) {
		avoidVector /= static_cast<float>(neighborCount);
	}

	return avoidVector;
}
