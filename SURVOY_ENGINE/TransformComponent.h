#pragma once

#include <glm/glm.hpp>  // Header for the GLM library
#include <glm/gtc/quaternion.hpp>  // Header for GLM's quaternion class
#include "Component.h"

namespace ECS {
	class TransformComponent : public Component
	{
    public:
        glm::vec3 position;             // Represents the position of the entity
        glm::quat rotation;             // Represents the rotation of the entity using quaternions
        glm::vec3 scale;                // Represents the scale of the entity
        glm::mat4 transformation;

        TransformComponent(
            const glm::vec3& pos = glm::vec3(0.0f),
            const glm::quat& rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            const glm::vec3& scl = glm::vec3(1.0f),
            const glm::mat4& transformation = glm::mat4(0)
        )
            : position(pos), rotation(rot), scale(scl), transformation(transformation) {}

        // You can add additional functionality here, like methods to manipulate the transform,
        // compute the transformation matrix, etc.
	};
}
