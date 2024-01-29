#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <type_traits>

namespace ECS {
    class EntityManager {
    private:
        int nextEntityId = 0;
        // Changed to nested map for type-indexed storage
        std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>> _entities;

    public:
        int getLastEntId() { return nextEntityId - 1; }

        int createEntity() {
            int entityId = nextEntityId++;
            _entities[entityId] = {};
            return entityId;
        }
         
        void destroyEntity(int entityId) {
            _entities.erase(entityId);
        }

        template<typename T, typename... TArgs>
        void addComponent(int entityId, TArgs&&... args) {
            auto component = std::make_shared<T>(std::forward<TArgs>(args)...);
            _entities[entityId][typeid(T)] = component;
        }

        template<typename T>
        T* getComponent(int entityId) {
            auto it = _entities.find(entityId);
            if (it != _entities.end()) {
                auto compIt = it->second.find(typeid(T));
                if (compIt != it->second.end()) {
                    return static_cast<T*>(compIt->second.get());
                }
            }
            return nullptr;
        }

        // Method to get all entities - adjust return type as needed
        auto& getEntities() {
            return _entities;
        }

        // Other EntityManager methods...
    };
}
