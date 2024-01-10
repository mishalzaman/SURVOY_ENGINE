#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include "PhysicsDebugDraw.h"
#include <memory>
#include <vector>
#include <iostream>
#include "SVertex.h"
#include "PhysicsCharacter.h"

namespace BAE {
	class Physics
	{
	public:
		Physics();
		~Physics();

		void Simulate(float deltaTime);
		void DrawDebug(glm::mat4 projection, glm::mat4 view);


		void CreateStaticShape(const std::vector<SVertex>& vertices);


		void IterateShapes(glm::vec3 forward, glm::vec3 velocity, float yaw);

		void CreatePlayerCapsule(PhysicsCharacter& character);
		void UpdatePlayerCapsule(PhysicsCharacter& character);
		void RetrieveVectorsPlayerCapsule(PhysicsCharacter& character);
	private:
		void _initialize();

		std::unique_ptr<btDefaultCollisionConfiguration> _collisionConfiguration;
		std::unique_ptr<btCollisionDispatcher> _dispatcher;
		std::unique_ptr<btDbvtBroadphase> _broadphase;
		std::unique_ptr<btSequentialImpulseConstraintSolver> _solver;
		std::unique_ptr<btDiscreteDynamicsWorld> _world;

		PhysicsDebugDraw _physicsDebugDraw;
		btAlignedObjectArray<btCollisionShape*> _collisionShapes;
		std::vector<std::shared_ptr<btTriangleMesh>> _triangleMeshes;

		btRigidBody* _character;
	};
}
