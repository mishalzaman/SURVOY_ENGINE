#include "KinematicCharacterControllerSystem.h"

ECS::KinematicCharacterControllerSystem::KinematicCharacterControllerSystem(EntityManager& entityManager, Physics& physics):
	_entityManager(entityManager), _physics(physics)
{
}

void ECS::KinematicCharacterControllerSystem::Load()
{
	int e = _entityManager.getIdByTag("CharacterController");

	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
	assert(orientation);

	orientation->Forward = ENGINE::VectorHelpers::ForwardVec3(orientation->Yaw, orientation->Pitch);
	orientation->Right = ENGINE::VectorHelpers::RightVec3(orientation->Forward);
	orientation->Up = ENGINE::VectorHelpers::UpVec3(orientation->Forward, orientation->Right);
}

void ECS::KinematicCharacterControllerSystem::UpdateOnFixedTimestep(float deltaTime)
{
	// Gravity
	ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(
		_entityManager.getIdByTag("CharacterController")
	);
	assert(kinematic);

	std::cout << _isOnGround(*kinematic) << std::endl;
	if (!_isOnGround(*kinematic)) {
		_handleGravity(*kinematic, deltaTime);
	}
}

void ECS::KinematicCharacterControllerSystem::Unload()
{
}

bool ECS::KinematicCharacterControllerSystem::_isOnGround(KinematicCapsulePhysicsBodyComponent& kinematic)
{
	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
		_entityManager.getIdByTag("CharacterController")
	);
	assert(orientation);

	if (orientation) {
		// Get the current position of the body
		btTransform transform;
		kinematic.Body->getMotionState()->getWorldTransform(transform);
		btVector3 currentPosition = transform.getOrigin();

		btVector3 start = currentPosition;
		btVector3 end = btVector3(currentPosition.x(), currentPosition.y() - 0.9f, currentPosition.z());

		btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);

		_physics.World().rayTest(start, end, rayCallback);

		btIDebugDraw* debugDrawer = _physics.World().getDebugDrawer();
		if (debugDrawer) {
			debugDrawer->drawLine(start, end, btVector3(1, 1, 1));
		}

		return rayCallback.hasHit();
	}

	return false;
}

void ECS::KinematicCharacterControllerSystem::_handleGravity(KinematicCapsulePhysicsBodyComponent& kinematic, float deltaTime)
{
	if (!kinematic.Body || !kinematic.Body->isKinematicObject()) {
		std::cout << "error not kinematic" << std::endl;
		return;
	}

	// Calculate the displacement due to gravity
	float displacement = 0.5f * GRAVITY * deltaTime * deltaTime;

	// Get the current position of the body
	btTransform transform;
	kinematic.Body->getMotionState()->getWorldTransform(transform);
	btVector3 currentPosition = transform.getOrigin();

	// Update the position to simulate gravity
	btVector3 newPosition = currentPosition + btVector3(0, displacement, 0);

	std::cout << transform.getOrigin().y() << std::endl;

	// Apply the new position
	transform.setOrigin(newPosition);
	kinematic.Body->getMotionState()->setWorldTransform(transform);
	kinematic.Body->setWorldTransform(transform);
}
