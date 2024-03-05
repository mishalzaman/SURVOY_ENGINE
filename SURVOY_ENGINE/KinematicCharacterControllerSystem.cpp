#include "KinematicCharacterControllerSystem.h"

ECS::KinematicCharacterControllerSystem::KinematicCharacterControllerSystem(EntityManager& entityManager, Physics& physics):
	_entityManager(entityManager), _physics(physics), _velocity(glm::vec3(0)), _acceleration(1), _verticalVelocity(glm::vec3(0))
{
}

ECS::KinematicCharacterControllerSystem::~KinematicCharacterControllerSystem()
{
}

void ECS::KinematicCharacterControllerSystem::Load()
{
	int e = _entityManager.getIdByTag("CharacterController");

	ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
	assert(orientation);

    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(kinematic);

	orientation->Forward = ENGINE::VectorHelpers::ForwardVec3(orientation->Yaw, orientation->Pitch);
	orientation->Right = ENGINE::VectorHelpers::RightVec3(orientation->Forward);
	orientation->Up = ENGINE::VectorHelpers::UpVec3(orientation->Forward, orientation->Right);

    _createGhostObject();
}

void ECS::KinematicCharacterControllerSystem::UpdateOnFixedTimestep(float deltaTime)
{
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(orientation);

    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(kinematic);

    btTransform transform;
    kinematic->Body->getMotionState()->getWorldTransform(transform);
    btVector3 currentPosition = transform.getOrigin();

    orientation->Position = glm::vec3(currentPosition.x(), currentPosition.y(), currentPosition.z());

    _ghostObject->setWorldTransform(transform);

    if (!_isOnGround()) {
        _handleGravity(*kinematic, deltaTime);
    }
    else {
        _verticalVelocity = glm::vec3(0);
        _move(deltaTime);
    }
}

void ECS::KinematicCharacterControllerSystem::Unload()
{
    _physics.World().removeCollisionObject(_ghostObject);
    delete _ghostObject;
}

void ECS::KinematicCharacterControllerSystem::_move(float deltaTime)
{
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(orientation);

    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(kinematic);

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    // Assuming orientation->dir is a forward vector and orientation->up is an up vector
    glm::vec3 forwardDir = orientation->Forward;
    glm::vec3 upDir = orientation->Up;

    // Modify horizontal velocity based on input
    glm::vec3 horizontalVelocity = glm::vec3(_velocity.x, 0, _velocity.z);
    bool isMovingForward = keystate[SDL_SCANCODE_W];
    bool isMovingBackward = keystate[SDL_SCANCODE_S];

    if (isMovingForward) {
        horizontalVelocity += forwardDir * _acceleration * deltaTime;
    }
    else if (isMovingBackward) {
        horizontalVelocity -= forwardDir * _acceleration * deltaTime;
    }
    else {
        // Apply deceleration if not moving forward or backward
        float currentSpeed = glm::length(horizontalVelocity);
        if (currentSpeed != 0) {
            // Calculate deceleration amount
            float decel = glm::min(currentSpeed, DECELERATION * deltaTime);
            horizontalVelocity -= decel * glm::normalize(horizontalVelocity);
        }
    }

    // Clamp horizontal velocity
    float horizontalSpeed = glm::length(horizontalVelocity);
    if (horizontalSpeed > SPEED) {
        horizontalVelocity = glm::normalize(horizontalVelocity) * SPEED;
    }

    // Update the full velocity vector, maintaining the vertical component
    _velocity.x = horizontalVelocity.x;
    _velocity.z = horizontalVelocity.z;

    // Combine with vertical velocity for displacement
    _velocity = _velocity + _verticalVelocity * deltaTime;

    // Clamp velocity to the maximum speed
    if (_velocity.length() > SPEED) {
        glm::normalize(_velocity);
        _velocity *= SPEED;
    }

    const float rotationSpeed = 40.f; // Adjust as needed for how fast you want the character to rotate

    if (keystate[SDL_SCANCODE_A]) {
        // Rotate left
        orientation->Yaw -= rotationSpeed * deltaTime;
    }
    if (keystate[SDL_SCANCODE_D]) {
        // Rotate right
        orientation->Yaw += rotationSpeed * deltaTime;
    }

    orientation->Forward = ENGINE::VectorHelpers::ForwardVec3(orientation->Yaw, orientation->Pitch);
    orientation->Right = ENGINE::VectorHelpers::RightVec3(orientation->Forward);
    orientation->Up = ENGINE::VectorHelpers::UpVec3(orientation->Forward, orientation->Right);

    // update position

    glm::vec3 displacement = _velocity;

    _updateKinematicPosition(displacement);
}


bool ECS::KinematicCharacterControllerSystem::_isOnGround()
{
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(orientation);

    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(kinematic);

    if (orientation && kinematic) {
        btIDebugDraw* debugDrawer = _physics.World().getDebugDrawer();
        debugDrawer->drawCapsule(kinematic->Radius * GHOST_OBJECT_SCALE, kinematic->Height * GHOST_OBJECT_SCALE * 0.5, 1, _ghostObject->getWorldTransform(), btVector3(0, 1, 0));

        // Optionally, add a raycast from the bottom of the capsule to further improve ground detection
        btVector3 rayStart = _ghostObject->getWorldTransform().getOrigin();
        btVector3 rayEnd = rayStart - btVector3(0, 1, 0) * (kinematic->Height * 0.5 + kinematic->Radius + GROUND_TEST_OFFSET);
        debugDrawer->drawLine(rayStart, rayEnd, btVector3(1, 1, 0));
        btCollisionWorld::ClosestRayResultCallback rayCallback(rayStart, rayEnd);
        _physics.World().rayTest(rayStart, rayEnd, rayCallback);
        if (rayCallback.hasHit()) {
            debugDrawer->drawSphere(rayCallback.m_hitPointWorld, 0.2f, btVector3(0, 0, 1));
            return true; // Raycast hit the ground, character is on ground
        }

        return false; // No contact is found, character is not on ground

        // Check for overlaps
        btManifoldArray manifoldArray;
        btBroadphasePairArray& pairArray = _ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
        int numPairs = pairArray.size();

        for (int i = 0; i < numPairs; i++) {
            manifoldArray.clear();

            const btBroadphasePair& pair = pairArray[i];

            // Unless we manually perform collision detection on this pair, the contacts are not generated
            btBroadphasePair* collisionPair = _physics.World().getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
            if (!collisionPair) continue;
            
            if (collisionPair->m_algorithm) {
                collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);
            }

            for (int j = 0; j < manifoldArray.size(); j++) {
                btPersistentManifold* manifold = manifoldArray[j];
                const btCollisionObject* obA = static_cast<const btCollisionObject*>(manifold->getBody0());
                const btCollisionObject* obB = static_cast<const btCollisionObject*>(manifold->getBody1());

                int numContacts = manifold->getNumContacts();
                for (int p = 0; p < numContacts; p++) {
                    const btManifoldPoint& pt = manifold->getContactPoint(p);
                    if (pt.getDistance() < 0.f) {
                        return true; // Contact is found, character is on ground
                    }
                }
            }
        }
    }

    return false; // No contact is found, character is not on ground
}

void ECS::KinematicCharacterControllerSystem::_handleGravity(KinematicCapsulePhysicsBodyComponent& kinematic, float deltaTime) {
    if (!kinematic.Body || !kinematic.Body->isKinematicObject()) {
        std::cout << "Error: Not a kinematic object" << std::endl;
        return;
    }

    // Assuming GRAVITY is defined elsewhere and is a negative value (e.g., -9.81)
    // Update the vertical velocity of the kinematic object due to gravity
    // Since _acceleration is scalar, and assuming you want to use it as gravity's magnitude,
    // you should directly use GRAVITY or set _acceleration to GRAVITY's value
    _verticalVelocity += GRAVITY * deltaTime; // Gravity affects the y-component of velocity

    // Calculate the displacement using the updated velocity
    // Displacement is velocity * time
    glm::vec3 displacement = _verticalVelocity * deltaTime;

    //// Get the current position of the body
    //btTransform transform;
    //kinematic.Body->getMotionState()->getWorldTransform(transform);
    //btVector3 currentPosition = transform.getOrigin();

    //// Convert displacement from glm::vec3 to btVector3 and update the position
    //btVector3 newPosition = currentPosition + btVector3(displacement.x, displacement.y, displacement.z);

    //// Apply the new position
    //transform.setOrigin(newPosition);
    //kinematic.Body->getMotionState()->setWorldTransform(transform);
    //kinematic.Body->setWorldTransform(transform);

    _updateKinematicPosition(displacement);
}

void ECS::KinematicCharacterControllerSystem::_resetVelocity()
{
    _velocity = glm::vec3(0);
}

void ECS::KinematicCharacterControllerSystem::_updateKinematicPosition(glm::vec3 displacement)
{
    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(kinematic);

    // Get the current position of the body
    btTransform transform;
    kinematic->Body->getMotionState()->getWorldTransform(transform);
    btVector3 currentPosition = transform.getOrigin();

    // Convert displacement from glm::vec3 to btVector3 and update the position
    btVector3 newPosition = currentPosition + btVector3(displacement.x, displacement.y, displacement.z);

    // Apply the new position
    transform.setOrigin(newPosition);
    kinematic->Body->getMotionState()->setWorldTransform(transform);
    kinematic->Body->setWorldTransform(transform);
}

void ECS::KinematicCharacterControllerSystem::_createGhostObject()
{
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(orientation);

    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(kinematic);

    if (orientation && kinematic) {
        btIDebugDraw* debugDrawer = _physics.World().getDebugDrawer();

        // Get the current position of the body
        btTransform transform;
        kinematic->Body->getMotionState()->getWorldTransform(transform);
        btVector3 currentPosition = transform.getOrigin();

        // Capsule dimensions (radius and height)
        float capsuleRadius = kinematic->Radius * GHOST_OBJECT_SCALE;
        float capsuleHeight = kinematic->Height * GHOST_OBJECT_SCALE;

        // Create a ghost object
        _ghostObject = new btPairCachingGhostObject();
        _ghostObject->setWorldTransform(transform);
        btCapsuleShape* capsuleShape = new btCapsuleShape(capsuleRadius, capsuleHeight);
        _ghostObject->setCollisionShape(capsuleShape);
        _ghostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

        // This should include static bodies in the collision detection
        int ghostCollisionGroup = btBroadphaseProxy::SensorTrigger;
        int ghostCollidesWith = btBroadphaseProxy::StaticFilter; // Only need to collide with static objects

        _physics.World().addCollisionObject(_ghostObject, ghostCollisionGroup, ghostCollidesWith);

        _physics.CollisionShapes().push_back(capsuleShape);

        std::cout << "Created ghost object" << std::endl;
    }
}
