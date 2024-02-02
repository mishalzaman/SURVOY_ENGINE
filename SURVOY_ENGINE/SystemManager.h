#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "System.h"
#include "EntityManager.h"
#include "Physics.h"

using namespace BAE;

namespace ECS {
    class SystemManager {
    private:
        std::unordered_map<std::type_index, std::shared_ptr<System>> systemMap;
        std::vector<std::shared_ptr<System>> systemList;

    public:
        SystemManager() {}

        template<typename T, typename... TArgs>
        void AddSystem(TArgs&&... args) {
            auto system = std::make_shared<T>(std::forward<TArgs>(args)...);
            systemMap[typeid(T)] = system;
            systemList.push_back(system);
            // Initialize the system if necessary
        }

        template<typename T>
        T* GetSystem() {
            auto it = systemMap.find(typeid(T));
            if (it != systemMap.end()) {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        void Load() {
            for (auto& system : systemList) {
                system->Load();
            }
        }

        void Update() {
            for (auto& system : systemList) {
                system->Update();
            }
        }

        void Update(float deltaTime) {
            for (auto& system : systemList) {
                system->Update(deltaTime);
            }
        }

        void Renders() {
            for (auto& system : systemList) {
                system->Renders();
            }
        }

        void Unload() {
            for (auto& system : systemList) {
                system->Unload();
            }
        }
    };
}
