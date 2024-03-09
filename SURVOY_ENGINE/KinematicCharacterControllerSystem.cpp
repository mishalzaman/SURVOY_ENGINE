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
    _updateVectors();
}

void ECS::KinematicCharacterControllerSystem::UpdateOnFixedTimestep(float deltaTime)
{
    int e = _entityManager.getIdByTag("CharacterController");

    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(e);
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);

    assert(orientation);
    assert(kinematic);
    assert(ghost);
    assert(motion);

    /*
    1. Get user input
    2. Get new velocity from user input
    3. handle gravity
    4. Compose a new displacement velocity and set the ghost object position 
    5. 
    
    /*==============
    USER INPUT
    ===============*/
    _turn(deltaTime);
    _updateVectors();
    _forwardBackward(deltaTime);

    /*==============
    GRAVITY
    ===============*/
    _handleGravity(deltaTime);

    /*==============
    GRAVITY
    ===============*/
    _updateGhostObjectPosition();

    /*==============
    PHYSICS TESTS
    ===============*/
    if (_isOnGround()) {
        motion->VerticalVelocity = glm::vec3(0);
    }
    _createDisplacement();

    // Update all positions
    _updateKinematicPosition();
    _updateEntityPosition();

    _IsNextToWall();
}

void ECS::KinematicCharacterControllerSystem::Unload()
{
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(ghost);

    _physics.World().removeCollisionObject(ghost->GhostObject);
    delete ghost->GhostObject;
}

glm::vec3 ECS::KinematicCharacterControllerSystem::_projectOnPlane(
    glm::vec3 pointA,
    glm::vec3 pointB,
    glm::vec3 normal
) {

    glm::vec3 A = pointA - pointB; // Vector from pointB to pointA
    normal = glm::normalize(normal); // Ensure the normal is normalized

    // Compute the projection of A onto the plane defined by normal
    glm::vec3 projection = A - glm::dot(A, normal) * normal;

    return projection;
}


void ECS::KinematicCharacterControllerSystem::_updateVectors()
{
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(orientation);

    orientation->Forward = ENGINE::VectorHelpers::ForwardVec3(orientation->Yaw, orientation->Pitch);
    orientation->Right = ENGINE::VectorHelpers::RightVec3(orientation->Forward);
    orientation->Up = ENGINE::VectorHelpers::UpVec3(orientation->Forward, orientation->Right);
}

/*/==============================
MOVEMENT UPDATES
================================*/

void ECS::KinematicCharacterControllerSystem::_forwardBackward(float deltaTime)
{
    int e = _entityManager.getIdByTag("CharacterController");

    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(e);
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);

    assert(orientation);
    assert(kinematic);
    assert(motion);

    motion->HorizontalVelocity = glm::vec3(0);

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    bool forward = keystate[SDL_SCANCODE_W];
    bool backward = keystate[SDL_SCANCODE_S];

    if (forward || backward) {
        if (forward) {
            motion->HorizontalVelocity += orientation->Forward * motion->Acceleration * deltaTime;
        }

        if (backward) {
            motion->HorizontalVelocity -= orientation->Forward * motion->Acceleration * deltaTime;
        }
    }

    // clamp
    float horizontalSpeed = glm::length(motion->HorizontalVelocity);
    if (horizontalSpeed > motion->Speed) {
        motion->HorizontalVelocity = glm::normalize(motion->HorizontalVelocity) * motion->Speed;
    }
}

void ECS::KinematicCharacterControllerSystem::_turn(float deltaTime)
{
    int e = _entityManager.getIdByTag("CharacterController");

    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);

    assert(orientation);
    assert(motion);

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_A]) {
        // Rotate left
        orientation->Yaw -= motion->TurnRate * deltaTime;
    }
    if (keystate[SDL_SCANCODE_D]) {
        // Rotate right
        orientation->Yaw += motion->TurnRate * deltaTime;
    }
}

void ECS::KinematicCharacterControllerSystem::_handleGravity(float deltaTime) {
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(motion);

    _verticalVelocity += motion->Gravity * deltaTime; // Gravity affects the y-component of velocity
    motion->VerticalVelocity = _verticalVelocity * deltaTime;
}

/*/==============================
POSITION UPDATES
================================*/

void ECS::KinematicCharacterControllerSystem::_updateKinematicPosition()
{
    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(kinematic);

    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(
        _entityManager.getIdByTag("CharacterController")
    );
    assert(motion);

    // Get the current position of the body
    btTransform transform;
    kinematic->Body->getMotionState()->getWorldTransform(transform);
    btVector3 currentPosition = transform.getOrigin();

    // Convert displacement from glm::vec3 to btVector3 and update the position
    glm::vec3 displacement = motion->Displacement;
    btVector3 newPosition = currentPosition + btVector3(displacement.x, displacement.y, displacement.z);

    // Apply the new position
    transform.setOrigin(newPosition);
    kinematic->Body->getMotionState()->setWorldTransform(transform);

    motion->Displacement = glm::vec3(0);
}

void ECS::KinematicCharacterControllerSystem::_updateGhostObjectPosition()
{
    int e = _entityManager.getIdByTag("CharacterController");

    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(e);
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);

    assert(kinematic);
    assert(ghost);

    // Get kinematic Position
    btTransform transform;
    kinematic->Body->getMotionState()->getWorldTransform(transform);

    // Set ghost object position
    ghost->GhostObject->setWorldTransform(transform);
}

void ECS::KinematicCharacterControllerSystem::_updateEntityPosition()
{
    int e = _entityManager.getIdByTag("CharacterController");

    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(e);
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);

    assert(kinematic);
    assert(orientation);

    // Get kinematic Position
    btTransform transform;
    kinematic->Body->getMotionState()->getWorldTransform(transform);
    btVector3 currentPosition = transform.getOrigin();

    // Set entity position
    orientation->Position = glm::vec3(currentPosition.x(), currentPosition.y(), currentPosition.z());
}

/*/==============================
PHYSICS TESTS
================================*/

bool ECS::KinematicCharacterControllerSystem::_isOnGround()
{
    int e = _entityManager.getIdByTag("CharacterController");
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(e);
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);

    assert(orientation);
    assert(kinematic);
    assert(ghost);

    btVector3 rayStart = ghost->GhostObject->getWorldTransform().getOrigin();
    btVector3 rayEnd = rayStart - btVector3(0, 1, 0) * (ghost->Height * 0.5 + ghost->Radius + GROUND_TEST_OFFSET);

    btCollisionWorld::ClosestRayResultCallback rayCallback(rayStart, rayEnd);

    _physics.World().rayTest(rayStart, rayEnd, rayCallback);
        
    if (rayCallback.hasHit()) {
        btIDebugDraw* debugDrawer = _physics.World().getDebugDrawer();
        debugDrawer->drawSphere(rayCallback.m_hitPointWorld, 0.2f, btVector3(0, 0, 1));
        return true;
    }
    
    return false;
}

bool ECS::KinematicCharacterControllerSystem::_isOnSlope()
{
    return false;
}

bool ECS::KinematicCharacterControllerSystem::_IsNextToWall()
{
    int e = _entityManager.getIdByTag("CharacterController");
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);
    assert(ghost);

    // Check for overlaps
    btManifoldArray manifoldArray;
    btBroadphasePairArray& pairArray = ghost->GhostObject->getOverlappingPairCache()->getOverlappingPairArray();
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
                    btIDebugDraw* debugDrawer = _physics.World().getDebugDrawer();
                    debugDrawer->drawSphere(pt.getPositionWorldOnA(), 0.2f, btVector3(0, 0, 1));
                    return true; // Contact is found, character is on ground
                }
            }
        }
    }

    return false;
}

void ECS::KinematicCharacterControllerSystem::_createDisplacement()
{
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(
        _entityManager.getIdByTag("CharacterController")
    );

    motion->Displacement = glm::vec3(
        motion->HorizontalVelocity.x,
        motion->VerticalVelocity.y,
        motion->HorizontalVelocity.z
    );
}

