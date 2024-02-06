#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <type_traits>
#include <deque>
#include <stdexcept>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <sstream>
#include <algorithm>

namespace ECS {
    class EntityManager {
    private:
        int nextEntityId = 1;
        std::deque<int> idPool; // Pool of reusable entity IDs
        // Contiguous storage for each component type
        std::unordered_map<std::type_index, std::vector<std::shared_ptr<void>>> componentArrays;
        // Mapping from entity IDs to component indices
        std::unordered_map<int, std::unordered_map<std::type_index, size_t>> entityComponentIndices;

        std::unordered_map<std::string, std::unordered_set<int>> tagToEntities; // Map from tag to entity IDs
        std::unordered_map<int, std::string> entityToTag; // Map from entity ID to its tag


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

            // Remove from tagging system
            if (entityToTag.find(entityId) != entityToTag.end()) {
                std::string tag = entityToTag[entityId];
                tagToEntities[tag].erase(entityId);
                if (tagToEntities[tag].empty()) {
                    tagToEntities.erase(tag);
                }
                entityToTag.erase(entityId);
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

        // Add a tag to an entity
        void addByTag(const std::string& tag, int entityId) {
            if (!entityComponentIndices.count(entityId)) {
                throw std::runtime_error("Entity ID does not exist");
            }
            tagToEntities[tag].insert(entityId);
            entityToTag[entityId] = tag;
        }

        // Retrieve entities by their tag
        std::vector<int> getByTag(const std::string& tag) {
            std::vector<int> entitiesWithTag;
            if (tagToEntities.find(tag) != tagToEntities.end()) {
                for (int entityId : tagToEntities[tag]) {
                    entitiesWithTag.push_back(entityId);
                }
            }
            return entitiesWithTag;
        }

        std::vector<int> getByTags(const std::string& tags) {
            std::vector<int> combinedEntities;
            std::stringstream tagStream(tags);
            std::string tag;

            while (getline(tagStream, tag, ',')) {
                // Trim leading and trailing spaces from tag if necessary
                tag.erase(0, tag.find_first_not_of(" \n\r\t"));
                tag.erase(tag.find_last_not_of(" \n\r\t") + 1);

                std::vector<int> entities = getByTag(tag); // Assuming this is the actual method fetching entities
                combinedEntities.insert(combinedEntities.end(), entities.begin(), entities.end());
            }

            // Optionally, remove duplicate entities if a single entity can have multiple tags
            std::sort(combinedEntities.begin(), combinedEntities.end());
            auto last = std::unique(combinedEntities.begin(), combinedEntities.end());
            combinedEntities.erase(last, combinedEntities.end());

            return combinedEntities;
        }
    };
}
