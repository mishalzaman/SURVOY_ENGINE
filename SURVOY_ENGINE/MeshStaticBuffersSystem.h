#pragma once

#include "System.h"
#include "EntityManager.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "BuffersComponent.h"
#include "TexturesComponent.h"

namespace ECS {
	class MeshStaticBuffersSystem : public System
	{
    public:
        MeshStaticBuffersSystem(EntityManager& entityManager);
        ~MeshStaticBuffersSystem();

        void Load() override;
        void Unload() override;

    private:
        EntityManager& _entityManager;
	};
}
