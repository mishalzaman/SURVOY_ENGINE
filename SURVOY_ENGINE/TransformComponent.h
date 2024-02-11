#pragma once

#include <glm/glm.hpp>  // Header for the GLM library
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>       // For glm::toMat4

namespace ECS {
    struct TransformComponent {
        glm::vec3 Position;
        glm::quat Rotation;
        glm::vec3 Scale;
        glm::mat4 Transformation;

        // Constructor that takes position, rotation, scale, and transformation matrix
        TransformComponent(const glm::mat4& trans)
            : Transformation(trans) {
            decompose();
        }

        void decompose() {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(Transformation, Scale, Rotation, Position, skew, perspective);
            Rotation = glm::conjugate(Rotation);
        }

        void compose() {
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);
            glm::mat4 rotationMatrix = glm::toMat4(Rotation);
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);

            // Combine the matrices: scale, then rotate, then translate
            Transformation = translationMatrix * rotationMatrix * scaleMatrix;
        }
    };
}
