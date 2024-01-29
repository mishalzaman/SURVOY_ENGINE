#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include "Component.h"

namespace ECS {
    class EntityManager {
    private:
        int nextEntityId = 0;
        std::unordered_map<int, std::vector<std::shared_ptr<Component>>> _entities;

    public:
        std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& getEntities() {
            return _entities;
        }

        int createEntity() {
            int entityId = nextEntityId++;
            _entities[entityId] = std::vector<std::shared_ptr<Component>>();
            return entityId;
        }

        void destroyEntity(int entityId) {
            _entities.erase(entityId);
        }

        template<typename T, typename... TArgs>
        void addComponent(int entityId, TArgs&&... args) {
            auto component = std::make_shared<T>(std::forward<TArgs>(args)...);
            _entities[entityId].push_back(component);
        }

        template<typename T>
        T* getComponent(int entityId) {
            auto it = _entities.find(entityId);
            if (it != _entities.end()) {
                for (auto& comp : it->second) {
                    if (typeid(*comp) == typeid(T)) {
                        return static_cast<T*>(comp.get());
                    }
                }
            }
            return nullptr;
        }

        // Other EntityManager methods...
    };
}
