#include "VectorHelpers.h"

glm::mat4 BAE::VectorHelpers::ViewMat4(const glm::vec3 position, const glm::vec3 forward, const glm::vec3 up)
{
    return glm::lookAt(position, position + forward, up);
}

glm::mat4 BAE::VectorHelpers::ProjectionMat4(const float screenW, const float screenH, const float fovDegrees)
{
    return glm::perspective(
        glm::radians(fovDegrees),
        (float)screenW / (float)screenH,
        NEAR,
        FAR
    );
}

glm::mat4 BAE::VectorHelpers::LookAtTargetViewMat4(const glm::vec3 target, const glm::vec3 position)
{
    glm::vec3 direction = glm::normalize(target - position);

    // Calculate pitch
    float pitch = glm::degrees(asin(direction.y));

    // Calculate yaw
    float yaw = glm::degrees(atan2(direction.z, direction.x));

    glm::vec3 forward = ForwardVec3(yaw, pitch);
    glm::vec3 right = RightVec3(forward);
    glm::vec3 up = UpVec3(forward, right);

    return ViewMat4(position, forward, up);
}

glm::vec3 BAE::VectorHelpers::ForwardVec3(const float yaw, const float pitch)
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    return glm::normalize(front);
}

glm::vec3 BAE::VectorHelpers::RightVec3(const glm::vec3 forward)
{
    return glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 BAE::VectorHelpers::UpVec3(const glm::vec3 forward, const glm::vec3 right)
{
    return glm::normalize(glm::cross(right, forward));
}
