#include "PlayerPhysicsSystem.h"

ECS::PlayerPhysicsSystem::PlayerPhysicsSystem(EntityManager& entityManager, Physics& physics):
	_entityManager(entityManager), _physics(physics)
{
}

void ECS::PlayerPhysicsSystem::UpdateOnFixedTimestep(float deltaTime)
{
    if (_contacts()) {
        std::cout << "penetration" << std::endl;
    }
    else {
        std::cout << "no penetration" << std::endl;
    }
    
}

bool ECS::PlayerPhysicsSystem::_contacts()
{
    int e = _entityManager.getIdByTag("CharacterController");
    ECS::GhostObjectCapsuleComponent* ghost = _entityManager.getComponent<ECS::GhostObjectCapsuleComponent>(e);
    assert(ghost);

    btIDebugDraw* debugDrawer = _physics.World().getDebugDrawer();

    btVector3 minAabb, maxAabb;
    ghost->GhostObject->getCollisionShape()->getAabb(
        ghost->GhostObject->getWorldTransform(), minAabb, maxAabb
    );
    _physics.World().getBroadphase()->setAabb(
        ghost->GhostObject->getBroadphaseHandle(),
        minAabb,
        maxAabb,
        _physics.World().getDispatcher()
    );

    bool penetration = false;

    _physics.World().getDispatcher()->dispatchAllCollisionPairs(
        ghost->GhostObject->getOverlappingPairCache(),
        _physics.World().getDispatchInfo(),
        _physics.World().getDispatcher()
    );
    btVector3 currentPosition = ghost->GhostObject->getWorldTransform().getOrigin();

    btManifoldArray manifoldArray;

    for (int i = 0; i < ghost->GhostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++) {
        manifoldArray.clear();

        btBroadphasePair* collisionPair = &ghost->GhostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];

        if (collisionPair->m_algorithm) {
            collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);
        }

        for (int j = 0; j < manifoldArray.size(); j++)
        {
            btPersistentManifold* manifold = manifoldArray[j];
            btScalar directionSign = manifold->getBody0() == ghost->GhostObject ? btScalar(-1.0) : btScalar(1.0);

            for (int p = 0; p < manifold->getNumContacts(); p++)
            {
                const btManifoldPoint& pt = manifold->getContactPoint(p);

                btScalar dist = pt.getDistance();

                if (dist < 0.0f)
                {
                    currentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar(0.2);
                    penetration = true;
                }
                else
                {
                    //std::cout << "no penetration" << std::endl;
                }

                // Draw a sphere at the contact point
                const btVector3& contactPoint = pt.getPositionWorldOnB();
                const btScalar sphereRadius = 0.1f;
                const btVector3 color(1.0f, 0.0f, 1.0f);
                debugDrawer->drawSphere(contactPoint, sphereRadius, color);
            }
        }
    }

    btTransform newTrans = ghost->GhostObject->getWorldTransform();
    newTrans.setOrigin(currentPosition);
    ghost->GhostObject->setWorldTransform(newTrans);

    return penetration;
}
