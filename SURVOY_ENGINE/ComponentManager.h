#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <cassert>
#include "Component.h"

namespace ECS {
    class ComponentManager {
    private:
        // Map from type index to a vector of components
        std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>> componentArrays;

    public:
        template<typename T, typename... TArgs>
        T* addComponent(int entityId, TArgs&&... args) {
            std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<TArgs>(args)...);
            getComponentArray<T>().emplace_back(newComponent);
            return newComponent.get();
        }

        template<typename T>
        std::vector<std::shared_ptr<Component>>& getComponentArray() {
            std::type_index typeIndex = typeid(T);
            if (componentArrays.find(typeIndex) == componentArrays.end()) {
                // Create a new vector for components of type T and return it
                componentArrays[typeIndex] = std::vector<std::shared_ptr<Component>>();
            }
            return componentArrays[typeIndex];
        }

        template<typename T>
        T* getComponent(int entityId) {
            auto& components = getComponentArray<T>();
            // Assuming entity ID corresponds to the index in the component array
            // This is a simplification and may need more sophisticated mapping
            if (entityId < components.size()) {
                return static_cast<T*>(components[entityId].get());
            }
            return nullptr;
        }

        // Other ComponentManager methods...
    };
}
