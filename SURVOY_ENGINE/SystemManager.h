#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "ISystem.h"
#include "EntityManager.h"
#include "Physics.h"

using namespace ENGINE;

namespace ECS {
    class SystemManager {
    private:
        std::unordered_map<std::type_index, std::shared_ptr<ISystem>> systemMap;
        std::vector<std::shared_ptr<ISystem>> systemList;

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

        void UpdatePrePhysics() {
            for (auto& system : systemList) {
                system->UpdatePrePhysics();
            }
        }

        void Update(float deltaTime) {
            for (auto& system : systemList) {
                system->Update(deltaTime);
            }
        }

        void UpdatePostPhysics() {
            for (auto& system : systemList) {
                system->UpdatePostPhysics();
            }
        }

        void Render() {
            for (auto& system : systemList) {
                system->Render();
            }
        }

        void Unload() {
            for (auto& system : systemList) {
                system->Unload();
            }
        }
    };
}
