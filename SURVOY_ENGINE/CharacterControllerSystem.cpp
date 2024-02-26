#include "CharacterControllerSystem.h"

ECS::CharacterControllerSystem::CharacterControllerSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager) :
    _eventManager(eventManager),
    _entityManager(entityManager),
    _physics(physics)
{
	_eventManager.subscribe(this);
}

ECS::CharacterControllerSystem::~CharacterControllerSystem()
{
	_eventManager.unsubscribe(this);
}

void ECS::CharacterControllerSystem::onNotify(const Event& event)
{
	const auto* yawEvent = dynamic_cast<const CameraYawEvent*>(&event);
	if (yawEvent) { _updateYaw(yawEvent->getYaw()); }
}

void ECS::CharacterControllerSystem::Load()
{
	std::vector<int> entities = _entityManager.getByTag("CharacterController");

	for (int entityId : entities) {
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);


		if (orientation) {
			orientation->Forward = ENGINE::VectorHelpers::ForwardVec3(orientation->Yaw, orientation->Pitch);
			orientation->Right = ENGINE::VectorHelpers::RightVec3(orientation->Forward);
			orientation->Up = ENGINE::VectorHelpers::UpVec3(orientation->Forward, orientation->Right);
		}
	}
}

void ECS::CharacterControllerSystem::UpdateOnFixedTimestep(float deltaTime)
{
	int e = _entityManager.getByTag("CharacterController")[0];

	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
	ECS::VelocityComponent* velocity = _entityManager.getComponent<ECS::VelocityComponent>(e);
	ECS::DynamicCapsulePhysicsBodyComponent* dynamic = _entityManager.getComponent<ECS::DynamicCapsulePhysicsBodyComponent>(e);
		
	if (orientation && velocity && dynamic) {
		_updateVectors(orientation->Yaw, orientation->Forward, orientation->Right, orientation->Up);
		_updateInput(deltaTime, orientation->Forward, orientation->Right, velocity->Velocity, velocity->Direction);
		_updatePhysics(*dynamic, velocity->Direction, velocity->Velocity);
	}
}

void ECS::CharacterControllerSystem::UpdateOnVariableTimestep()
{
	int e = _entityManager.getByTag("CharacterController")[0];

	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
	ECS::DynamicCapsulePhysicsBodyComponent* dynamic = _entityManager.getComponent<ECS::DynamicCapsulePhysicsBodyComponent>(e);


	if (orientation && dynamic) {

		btTransform trans;
		if (dynamic->Body->getMotionState()) {
			dynamic->Body->getMotionState()->getWorldTransform(trans);
			orientation->Position = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

			_eventManager.notifyAll(CharacterControllerPositionEvent(orientation->Position));
		}
	}
}

void ECS::CharacterControllerSystem::Render()
{
	std::vector<int> entities = _entityManager.getByTag("CharacterController");

	for (int entityId : entities) {
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

		if (orientation) {
			btVector3 position = btVector3(orientation->Position.x, orientation->Position.y, orientation->Position.z);

			// Assuming the magnitude of vectors is suitable for visualization; otherwise, scale them as needed
			btVector3 forwardEnd = position + btVector3(orientation->Forward.x, orientation->Forward.y, orientation->Forward.z) * 2;
			btVector3 rightEnd = position + btVector3(orientation->Right.x, orientation->Right.y, orientation->Right.z) * 2;
			btVector3 upEnd = position + btVector3(orientation->Up.x, orientation->Up.y, orientation->Up.z) * 2;

			// Colors for each vector
			btVector3 forwardColor(1, 0, 0); // Red for Forward
			btVector3 rightColor(0, 1, 0); // Green for Right
			btVector3 upColor(0, 0, 1); // Blue for Up

			// Drawing the vectors
			_physics.World().getDebugDrawer()->drawLine(position, forwardEnd, forwardColor);
			_physics.World().getDebugDrawer()->drawLine(position, rightEnd, rightColor);
			_physics.World().getDebugDrawer()->drawLine(position, upEnd, upColor);	
		}
	}
}

void ECS::CharacterControllerSystem::_updateInput(float deltaTime, const glm::vec3& forward, const glm::vec3& right, float& velocity, glm::vec3& direction)
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	direction = glm::vec3(0);

	if (keystate[SDL_SCANCODE_W]) {
		direction += forward;
	}
	if (keystate[SDL_SCANCODE_S]) {
		direction -= forward;
	}
	if (keystate[SDL_SCANCODE_A]) {
		direction -= right;
	}
	if (keystate[SDL_SCANCODE_D]) {
		direction += right;
	}

	if (glm::length(direction) > 0) {
		direction = glm::normalize(direction);
		velocity = ACCELERATION * SPEED * deltaTime;
	}
}

void ECS::CharacterControllerSystem::_updateYaw(float yaw)
{
	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
		_entityManager.getByTag("CharacterController")[0]
	);

	if (orientation) { orientation->Yaw = yaw; }
	
}

void ECS::CharacterControllerSystem::_updateVectors(const float yaw, glm::vec3& forward, glm::vec3& right, glm::vec3& up)
{
	forward = ENGINE::VectorHelpers::ForwardVec3(yaw, 0.f);
	right = ENGINE::VectorHelpers::RightVec3(forward);
	up = ENGINE::VectorHelpers::UpVec3(forward, right);
}

void ECS::CharacterControllerSystem::_updatePhysics(ECS::DynamicCapsulePhysicsBodyComponent& dynamic, const glm::vec3& direction, const float& velocity)
{
	if (glm::length(direction) > 0) {
		dynamic.Body->activate(true);
		dynamic.Body->setLinearVelocity(btVector3(direction.x, direction.y, direction.z) * velocity);
	}
}
