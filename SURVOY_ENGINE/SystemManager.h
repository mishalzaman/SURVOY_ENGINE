#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include "System.h"
#include "Component.h"

namespace ECS {
    class SystemManager {
    private:
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
        std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities;

    public:
        SystemManager(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities)
            : entities(entities) {}

        template<typename T, typename... TArgs>
        void AddSystem(TArgs&&... args) {
            std::shared_ptr<T> system = std::make_shared<T>(std::forward<TArgs>(args)...);
            systems[typeid(T)] = system;
        }

        template<typename T>
        T* GetSystem() {
            auto it = systems.find(typeid(T));
            if (it != systems.end()) {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        void UpdateSystems(float deltaTime) {
            for (auto& [type, system] : systems) {
                system->Update(deltaTime, entities);
            }
        }

        void LoadSystems() {
            for (auto& [type, system] : systems) {
                system->Load(entities);
            }
        }
    };
}
