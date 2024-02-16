#ifndef PHYSICS_H
#define PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include "PhysicsDebugDraw.h"
#include <memory>
#include <vector>
#include <iostream>
#include "SVertex.h"

namespace ENGINE {
	class Physics
	{
	public:
		Physics();
		~Physics();

		void DrawDebug();

		void Simulate(float deltaTime);

		void StaticTriangleMesh(const std::vector<SVertex>& vertices, glm::mat4 tranformation);

		btDiscreteDynamicsWorld& World();
		btAlignedObjectArray<btCollisionShape*>& CollisionShapes() { return _collisionShapes; }

		void View(glm::mat4 view) { _view = view; }
		void Projection(glm::mat4 projection) { _projection = projection; }
		glm::mat4 View() { return _view; }
		glm::mat4 Projection() { return _projection; }

	private:
		std::unique_ptr<btDefaultCollisionConfiguration> _collisionConfiguration;
		std::unique_ptr<btCollisionDispatcher> _dispatcher;
		std::unique_ptr<btDbvtBroadphase> _broadphase;
		std::unique_ptr<btSequentialImpulseConstraintSolver> _solver;
		std::unique_ptr<btDiscreteDynamicsWorld> _world;

		PhysicsDebugDraw _physicsDebugDraw;
		btAlignedObjectArray<btCollisionShape*> _collisionShapes;

		glm::mat4 _view;
		glm::mat4 _projection;
	};
}

#endif