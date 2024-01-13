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

		void CreateLevelGeometry(const std::vector<SVertex>& vertices);
		void CreateLevelGeometry(const std::vector<SVertex>& vertices, glm::mat4 tranformation);

		void CreatePlayerGeometry(glm::vec3 position, float yaw, float pitch);
		void UpdatePlayerGeometry(glm::vec3 velocity, glm::vec3 forward);
		glm::vec3 PlayerPosition();
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
