#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include "Component.h"

namespace ECS {
    class System {
    public:
        virtual ~System() = default; // Virtual destructor for proper cleanup of derived types

        // Update method to be implemented by derived systems
        virtual void Update(float deltaTime, std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities) = 0;
        virtual void Load(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities) = 0;

        // Add other common system functionality here if needed
    };
}
