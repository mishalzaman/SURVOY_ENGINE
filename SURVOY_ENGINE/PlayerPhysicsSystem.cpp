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
                }
            }
        }
    }

    // Apply the accumulated velocity adjustment to the character's motion component.
    btVector3 initialVelocity = btVector3(motion->Velocity.x, motion->Velocity.y, motion->Velocity.z);
    btVector3 newVelocity = initialVelocity + velocityAdjustment;
    motion->Velocity = glm::vec3(newVelocity.getX(), newVelocity.getY(), newVelocity.getZ());

    // Depending on your game's requirements, you might want to further refine how the velocity adjustment is applied,
    // for example, by factoring in restitution or friction coefficients, or by applying different adjustments based on the type of collision.

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
