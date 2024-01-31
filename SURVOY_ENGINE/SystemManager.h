#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include "System.h"
#include "Physics.h"

using namespace BAE;

namespace ECS {
    class SystemManager {
    private:
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
        EntityManager& entityManager; // Reference to EntityManager
        Physics& physics;

    public:
        SystemManager(EntityManager& entityManager, Physics& physics)
            : entityManager(entityManager), physics(physics) {}

        template<typename T, typename... TArgs>
        void AddSystem(TArgs&&... args) {
            std::shared_ptr<T> system = std::make_shared<T>(std::forward<TArgs>(args)...);
            systems[typeid(T)] = system;
            // Initialize the system if necessary
        }

        template<typename T>
        T* GetSystem() {
            auto it = systems.find(typeid(T));
            if (it != systems.end()) {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        void Load() {
            for (auto& [type, system] : systems) {
                system->Load(entityManager, physics);
            }
        }

        void Update() {
            for (auto& [type, system] : systems) {
                system->Update(entityManager, physics);
            }
        }

        void Update(float deltaTime) {
            for (auto& [type, system] : systems) {
                system->Update(deltaTime, entityManager, physics);
            }
        }

        void Renders() {
            for (auto& [type, system] : systems) {
                system->Renders(entityManager);
            }
        }

        void Unload() {
            for (auto& [type, system] : systems) {
                system->Unload(entityManager, physics);
            }
        }

        void UpdateVec3(float x, float y, float z) {
            for (auto& [type, system] : systems) {
                system->UpdateVec3(entityManager, x, y, z);
            }
        }
    };
}
