#include "KinematicCharacterControllerSystem.h"

ECS::KinematicCharacterControllerSystem::KinematicCharacterControllerSystem(EntityManager& entityManager, Physics& physics):
	_entityManager(entityManager), _physics(physics), _velocity(glm::vec3(0)), _acceleration(1)
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
        std::cout <<  "on ground" << std::endl;
    }
}

void ECS::KinematicCharacterControllerSystem::Unload()
{
    _physics.World().removeCollisionObject(_ghostObject);
    delete _ghostObject;
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
    _velocity += GRAVITY * deltaTime; // Gravity affects the y-component of velocity

    // Calculate the displacement using the updated velocity
    // Displacement is velocity * time
    glm::vec3 displacement = _velocity * deltaTime;

    // Get the current position of the body
    btTransform transform;
    kinematic.Body->getMotionState()->getWorldTransform(transform);
    btVector3 currentPosition = transform.getOrigin();

    // Convert displacement from glm::vec3 to btVector3 and update the position
    btVector3 newPosition = currentPosition + btVector3(displacement.x, displacement.y, displacement.z);

    // Apply the new position
    transform.setOrigin(newPosition);
    kinematic.Body->getMotionState()->setWorldTransform(transform);
    kinematic.Body->setWorldTransform(transform);
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
