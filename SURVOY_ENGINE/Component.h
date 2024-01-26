#pragma once

namespace ECS {
    class Component {
    public:
        virtual ~Component() = default; // Virtual destructor for proper cleanup of derived types

        // You can add common functionality for all components here if needed.
        // However, in many ECS implementations, the Component class is often kept empty,
        // serving mainly as a base type for polymorphism.
    };
}
