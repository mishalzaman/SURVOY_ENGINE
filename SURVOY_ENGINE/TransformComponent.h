#pragma once

#include <glm/glm.hpp>  // Header for the GLM library
#include <glm/gtc/quaternion.hpp>  // Header for GLM's quaternion class

namespace ECS {
    struct TransformComponent {
        glm::vec3 Position;
        glm::quat Rotation;
        glm::vec3 Scale;
        glm::mat4 Transformation;

        // Constructor that takes position, rotation, scale, and transformation matrix
        TransformComponent(
            const glm::vec3& pos,
            const glm::quat& rot,
            const glm::vec3& scl,
            const glm::mat4& trans)
            : Position(pos), Rotation(rot), Scale(scl), Transformation(trans) {}
    };
}
