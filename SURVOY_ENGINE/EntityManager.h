#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <type_traits>
#include <deque>
#include <stdexcept>
#include <iostream>

namespace ECS {
    class EntityManager {
    private:
        int nextEntityId = 1;
        std::deque<int> idPool; // Pool of reusable entity IDs
        // Contiguous storage for each component type
        std::unordered_map<std::type_index, std::vector<std::shared_ptr<void>>> componentArrays;
        // Mapping from entity IDs to component indices
        std::unordered_map<int, std::unordered_map<std::type_index, size_t>> entityComponentIndices;

    public:
        int createEntity() {
            int entityId;
            if (!idPool.empty()) {
                // Reuse an ID from the pool
                entityId = idPool.front();
                idPool.pop_front();
            }
            else {
                // Allocate a new ID
                entityId = nextEntityId++;
            }
            entityComponentIndices[entityId] = {}; // Initialize the entry

            return entityId;
        }

        void destroyEntity(int entityId) {
            auto entityIter = entityComponentIndices.find(entityId);
            if (entityIter != entityComponentIndices.end()) {
                for (auto& [type, index] : entityIter->second) {
                    auto& components = componentArrays[type];

                    // Check if the component to remove is not the last one
                    if (index != components.size() - 1) {
                        // Swap with the last component
                        std::swap(components[index], components.back());

                        // Update the index of the swapped component
                        for (auto& [eId, compMap] : entityComponentIndices) {
                            if (compMap[type] == components.size() - 1) {
                                compMap[type] = index;
                                break;
                            }
                        }
                    }

                    // Remove the last component (either the original or the swapped one)
                    components.pop_back();
                }
                entityComponentIndices.erase(entityId); // Remove entity from map
            }
            idPool.push_back(entityId); // Recycle ID
        }


        // Template function to add a component of a specific type
        template<typename T, typename... TArgs>
        std::shared_ptr<T> addComponent(int entityId, TArgs&&... args) {
            // Check if entity exists
            if (entityComponentIndices.count(entityId) == 0) {
                throw std::runtime_error("Entity ID does not exist");
            }
            auto& entityComponents = entityComponentIndices[entityId];
            auto typeIndex = std::type_index(typeid(T));

            // Check for component duplication
            if (entityComponents.find(typeIndex) != entityComponents.end()) {
                // Handle duplication (e.g., return existing component or error)
                throw std::runtime_error("Component of this type already exists for the entity");
            }

            // Create the component
            auto component = std::make_shared<T>(std::forward<TArgs>(args)...);

            // Add component to the array and update the index map
            componentArrays[typeIndex].push_back(component);
            entityComponents[typeIndex] = componentArrays[typeIndex].size() - 1;

            return component;
        }


        // Template function to get a component of a specific type
        template<typename T>
        T* getComponent(int entityId) {
            std::type_index typeIndex(typeid(T));  // Create a type_index object
            auto it = entityComponentIndices.find(entityId);
            if (it != entityComponentIndices.end()) {
                auto compIt = it->second.find(typeIndex);
                if (compIt != it->second.end()) {
                    return static_cast<T*>(componentArrays[typeIndex][compIt->second].get());
                }
            }
            return nullptr;
        }


        // Returns the component pools for all types
        auto& getComponentPools() {
            return componentArrays;
        }

        // Returns the entity-component mapping
        auto& getEntityComponentIndices() {
            return entityComponentIndices;
        }

        // Returns both the entity-component mapping and component arrays
        auto getEntities() -> std::pair<
            const std::unordered_map<int, std::unordered_map<std::type_index, size_t>>&,
            const std::unordered_map<std::type_index, std::vector<std::shared_ptr<void>>>&> {

            return std::make_pair(std::ref(entityComponentIndices), std::ref(componentArrays));
        }
    };
}
