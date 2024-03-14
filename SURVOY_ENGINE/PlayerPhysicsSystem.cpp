#include "PlayerPhysicsSystem.h"

ECS::PlayerPhysicsSystem::PlayerPhysicsSystem(EntityManager& entityManager, Physics& physics):
	_entityManager(entityManager), _physics(physics)
{
}

void ECS::PlayerPhysicsSystem::UpdateOnFixedTimestep(float deltaTime)
{
    int e = _entityManager.getIdByTag("CharacterController");
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);

    if (_onGround()) {
        //motion->Velocity.y = 0;
    }
    _contacts();
}

bool ECS::PlayerPhysicsSystem::_contacts()
{
    int e = _entityManager.getIdByTag("CharacterController");
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);
    ECS::MovementAttributesComponent* motion = _entityManager.getComponent<ECS::MovementAttributesComponent>(e);
    assert(ghost && motion);

    btIDebugDraw* debugDrawer = _physics.World().getDebugDrawer();

    _originalPosition = _getGhostObjectOriginalPosition();
    _updateGhostObjectPosition(motion->Velocity);

    btVector3 minAabb, maxAabb;
    ghost->GhostObject->getCollisionShape()->getAabb(ghost->GhostObject->getWorldTransform(), minAabb, maxAabb);
    _physics.World().getBroadphase()->setAabb(ghost->GhostObject->getBroadphaseHandle(), minAabb, maxAabb, _physics.World().getDispatcher());

    bool penetration = false;
    btVector3 velocityAdjustment(0, 0, 0);

    _physics.World().getDispatcher()->dispatchAllCollisionPairs(
        ghost->GhostObject->getOverlappingPairCache(),
        _physics.World().getDispatchInfo(),
        _physics.World().getDispatcher()
    );

    btManifoldArray manifoldArray;
    for (int i = 0; i < ghost->GhostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++) {
        manifoldArray.clear();
        btBroadphasePair* collisionPair = &ghost->GhostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];

        if (collisionPair->m_algorithm) {
            collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);
        }

        for (int j = 0; j < manifoldArray.size(); j++) {
            btPersistentManifold* manifold = manifoldArray[j];

            for (int p = 0; p < manifold->getNumContacts(); p++) {
                const btManifoldPoint& pt = manifold->getContactPoint(p);
                btScalar dist = pt.getDistance();

                if (dist < 0.0f) {
                    btVector3 correctionDirection = pt.m_normalWorldOnB * dist * btScalar(-0.8);
                    velocityAdjustment += correctionDirection;
                    penetration = true;

                    // Draw a sphere at the contact point for visualization
                    const btVector3& contactPoint = pt.getPositionWorldOnB();
                    const btScalar sphereRadius = 0.1f;
                    const btVector3 color(1.0f, 0.0f, 1.0f);
                    debugDrawer->drawSphere(contactPoint, sphereRadius, color);

                    // Draw the normal at the contact point
                    btVector3 normal = pt.m_normalWorldOnB.normalized(); // Ensure the normal is normalized
                    float r = (normal.getX() + 1.0f) * 0.5f;
                    float g = (normal.getY() + 1.0f) * 0.5f;
                    float b = (normal.getZ() + 1.0f) * 0.5f;

                    const btVector3 normalColor(r, g, b);
                    const btVector3 normalEnd = contactPoint + pt.m_normalWorldOnB * sphereRadius * 20;
                    debugDrawer->drawLine(contactPoint, normalEnd, normalColor);

                }
            }
        }
    }

    // Apply the accumulated velocity adjustment to the character's motion component.
    btVector3 initialVelocity = btVector3(motion->Velocity.x, motion->Velocity.y, motion->Velocity.z);
    btVector3 newVelocity = initialVelocity + velocityAdjustment;
    motion->Velocity = glm::vec3(newVelocity.getX(), newVelocity.getY(), newVelocity.getZ());

    // reset ghost object to original position
    btTransform transform = ghost->GhostObject->getWorldTransform();
    transform.setOrigin(_originalPosition);
    ghost->GhostObject->setWorldTransform(transform);

    _updateGhostObjectPosition(motion->Velocity);

    return penetration;
}

bool ECS::PlayerPhysicsSystem::_onGround()
{
    int e = _entityManager.getIdByTag("CharacterController");
    ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(e);
    ECS::KinematicCapsulePhysicsBodyComponent* kinematic = _entityManager.getComponent<ECS::KinematicCapsulePhysicsBodyComponent>(e);
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);

    assert(orientation);
    assert(kinematic);
    assert(ghost);

    btVector3 rayStart = ghost->GhostObject->getWorldTransform().getOrigin();
    btVector3 rayEnd = rayStart - btVector3(0, 1, 0) * (ghost->Height * 0.5 + ghost->Radius + 0.015f);

    btCollisionWorld::ClosestRayResultCallback rayCallback(rayStart, rayEnd);

    _physics.World().rayTest(rayStart, rayEnd, rayCallback);

    if (rayCallback.hasHit()) {
        btIDebugDraw* debugDrawer = _physics.World().getDebugDrawer();
        debugDrawer->drawSphere(rayCallback.m_hitPointWorld, 0.2f, btVector3(0, 0, 1));
        return true;
    }

    return false;
}

void ECS::PlayerPhysicsSystem::_updateGhostObjectPosition(glm::vec3 velocity)
{
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(
        _entityManager.getIdByTag("CharacterController")
    );

    // Convert the velocity (now effectively displacement) to Bullet's vector type.
    btVector3 displacement = btVector3(velocity.x, velocity.y, velocity.z);

    // Get the current position of the ghost object.
    btTransform transform = ghost->GhostObject->getWorldTransform();
    btVector3 currentPosition = transform.getOrigin();

    // Calculate the new position by adding the displacement to the current position.
    btVector3 newPosition = currentPosition + displacement;

    // Update the ghost object's transform with the new position.
    transform.setOrigin(newPosition);
    ghost->GhostObject->setWorldTransform(transform);
}

btVector3 ECS::PlayerPhysicsSystem::_getGhostObjectOriginalPosition()
{
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(
        _entityManager.getIdByTag("CharacterController")
    );

    // Get the world transform of the ghost object.
    const btTransform& transform = ghost->GhostObject->getWorldTransform();
    // Extract the position (origin) from the transform.
    return transform.getOrigin();
}
