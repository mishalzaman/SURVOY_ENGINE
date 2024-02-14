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
	//const auto* yawEvent = dynamic_cast<const CameraYawEvent*>(&event);
	//if (yawEvent) { _updateYaw(yawEvent->getYaw()); }
}

void ECS::CharacterControllerSystem::Load()
{
	std::vector<int> entities = _entityManager.getByTag("PlayerController");

	for (int entityId : entities) {
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);


		if (orientation) {
			orientation->Forward = BAE::VectorHelpers::ForwardVec3(orientation->Yaw, orientation->Pitch);
			orientation->Right = BAE::VectorHelpers::RightVec3(orientation->Forward);
			orientation->Up = BAE::VectorHelpers::UpVec3(orientation->Forward, orientation->Right);
		}
	}
}

void ECS::CharacterControllerSystem::UpdatePrePhysics()
{

}

void ECS::CharacterControllerSystem::Update(float deltaTime)
{
	int e = _entityManager.getByTag("PlayerController")[0];

	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
	ECS::VelocityComponent* velocity = _entityManager.getComponent<ECS::VelocityComponent>(e);
	ECS::DynamicCapsulePhysicsBodyComponent* dynamic = _entityManager.getComponent<ECS::DynamicCapsulePhysicsBodyComponent>(e);
		
	if (orientation && velocity && dynamic) {
		float deltaYaw = 0.f;

		_updateVectors(orientation->Yaw, orientation->Forward, orientation->Right, orientation->Up);
		//_updateInput(deltaTime, orientation->Forward, orientation->Right, velocity->Velocity, velocity->Direction);
		_updateInput(deltaTime, orientation->Forward, velocity->Velocity, velocity->Direction, deltaYaw);
		_updatePhysics(*dynamic, velocity->Direction, velocity->Velocity, deltaYaw);
	}
}

void ECS::CharacterControllerSystem::UpdatePostPhysics()
{
	int e = _entityManager.getByTag("PlayerController")[0];

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
	std::vector<int> entities = _entityManager.getByTag("PlayerController");

	for (int entityId : entities) {
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

		if (orientation) {
			btVector3 position = btVector3(orientation->Position.x, orientation->Position.y, orientation->Position.z);

			// Assuming the magnitude of vectors is suitable for visualization; otherwise, scale them as needed
			btVector3 forwardEnd = position + btVector3(orientation->Forward.x, orientation->Forward.y, orientation->Forward.z);
			btVector3 rightEnd = position + btVector3(orientation->Right.x, orientation->Right.y, orientation->Right.z);
			btVector3 upEnd = position + btVector3(orientation->Up.x, orientation->Up.y, orientation->Up.z);

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

void ECS::CharacterControllerSystem::Unload()
{
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

void ECS::CharacterControllerSystem::_updateInput(float deltaTime, const glm::vec3& forward, float& velocity, glm::vec3& direction, float& deltaYaw)
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	direction = glm::vec3(0);
	deltaYaw = 0.0f; // Reset deltaYaw each frame

	if (keystate[SDL_SCANCODE_W]) {
		direction += forward;
	}
	if (keystate[SDL_SCANCODE_S]) {
		direction -= forward;
	}
	if (keystate[SDL_SCANCODE_A]) {
		deltaYaw -= YAW_SPEED * deltaTime;
	}
	if (keystate[SDL_SCANCODE_D]) {
		deltaYaw += YAW_SPEED * deltaTime;
	}

	if (glm::length(direction) > 0) {
		direction = glm::normalize(direction);
		velocity = ACCELERATION * SPEED * deltaTime;
	}
}

void ECS::CharacterControllerSystem::_updateYaw(float yaw)
{
	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
		_entityManager.getByTag("PlayerController")[0]
	);

	if (orientation) { orientation->Yaw = yaw; }
	
}

void ECS::CharacterControllerSystem::_updateVectors(const float yaw, glm::vec3& forward, glm::vec3& right, glm::vec3& up)
{
	forward = BAE::VectorHelpers::ForwardVec3(yaw, 0.f);
	right = BAE::VectorHelpers::RightVec3(forward);
	up = BAE::VectorHelpers::UpVec3(forward, right);
}

void ECS::CharacterControllerSystem::_updatePhysics(ECS::DynamicCapsulePhysicsBodyComponent& dynamic, const glm::vec3& direction, const float& velocity, const float& deltaYaw)
{
	if (glm::length(direction) > 0) {
		dynamic.Body->activate(true);
		dynamic.Body->setLinearVelocity(btVector3(direction.x, direction.y, direction.z) * velocity);
	}

	// Apply angular velocity based on deltaYaw. deltaYaw is in radians per second here.
	dynamic.Body->setAngularVelocity(btVector3(0, deltaYaw, 0));
}