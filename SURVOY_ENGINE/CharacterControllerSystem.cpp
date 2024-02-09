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
}

void ECS::CharacterControllerSystem::Load()
{
	std::vector<int> entities = _entityManager.getByTag("Player Controller");

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
	float yaw = 0.f;
	std::vector<int> entitiesCTP = _entityManager.getByTag("Camera Third Person");

	for (int entityId : entitiesCTP) {
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

		if (orientation) {
			yaw = orientation->Yaw;
		}
	}

	glm::vec3 newPosition = glm::vec3(0);
	std::vector<int> entities = _entityManager.getByTag("Player Controller");

	for (int entityId : entities) {
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);
		ECS::VelocityComponent* velocity = _entityManager.getComponent<ECS::VelocityComponent>(entityId);
		ECS::DynamicCapsulePhysicsBodyComponent* dynamic = _entityManager.getComponent<ECS::DynamicCapsulePhysicsBodyComponent>(entityId);
		

		if (orientation && velocity && dynamic) {
			orientation->Forward = BAE::VectorHelpers::ForwardVec3(yaw, 0.f);
			orientation->Right = BAE::VectorHelpers::RightVec3(orientation->Forward);
			orientation->Up = BAE::VectorHelpers::UpVec3(orientation->Forward, orientation->Right);

			_move(deltaTime, orientation->Forward, orientation->Right, velocity->Velocity, velocity->Direction);

			// Set the velocity of the physical character
			dynamic->Body->activate(true);
			dynamic->Body->setLinearVelocity(btVector3(velocity->Direction.x, velocity->Direction.y, velocity->Direction.z) * velocity->Velocity);
		}
	}
}

void ECS::CharacterControllerSystem::Render()
{
	std::vector<int> entities = _entityManager.getByTag("Player Controller");

	for (int entityId : entities) {
		ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

		if (orientation) {
			btVector3 position = btVector3(orientation->Position.x, orientation->Position.y + 1, orientation->Position.z);

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

void ECS::CharacterControllerSystem::Unload()
{
}

void ECS::CharacterControllerSystem::_move(float deltaTime, const glm::vec3& forward, const glm::vec3& right, float& velocity, glm::vec3& direction)
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
