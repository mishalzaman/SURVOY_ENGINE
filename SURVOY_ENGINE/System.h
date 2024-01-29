#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>

namespace ECS {
    class System {
    public:
        virtual ~System() = default; // Virtual destructor for proper cleanup of derived types

        // Update method signatures to match the new entity-component storage structure
        virtual void Load(std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>>& entities) = 0;
        virtual void Physics(float deltaTime, std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>>& entities) = 0;
        virtual void Renders(std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>>& entities) = 0;
        virtual void Unload(std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<void>>>& entities) = 0;

        // Add other common system functionality here if needed
    };
}
