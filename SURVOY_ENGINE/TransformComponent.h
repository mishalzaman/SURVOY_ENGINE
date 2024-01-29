#pragma once

#include <glm/glm.hpp>  // Header for the GLM library
#include <glm/gtc/quaternion.hpp>  // Header for GLM's quaternion class

namespace ECS {
    struct TransformComponent {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        glm::mat4 transformation;

        // Constructor that takes position, rotation, scale, and transformation matrix
        TransformComponent(
            const glm::vec3& pos,
            const glm::quat& rot,
            const glm::vec3& scl,
            const glm::mat4& trans)
            : position(pos), rotation(rot), scale(scl), transformation(trans) {}
    };
}
