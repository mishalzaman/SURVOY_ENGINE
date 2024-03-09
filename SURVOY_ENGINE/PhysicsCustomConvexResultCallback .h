#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

class PhysicsCustomConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback {
public:
    btCollisionObject* m_me;

    PhysicsCustomConvexResultCallback(const btVector3& convexFromWorld, const btVector3& convexToWorld, btCollisionObject* me)
        : btCollisionWorld::ClosestConvexResultCallback(convexFromWorld, convexToWorld), m_me(me) {}

    virtual bool needsCollision(btBroadphaseProxy* proxy) const {
        // Base class needCollision
        if (!btCollisionWorld::ClosestConvexResultCallback::needsCollision(proxy))
            return false;

        // Additional check to ignore the kinematic body itself
        return (proxy->m_clientObject != m_me);
    }
};
