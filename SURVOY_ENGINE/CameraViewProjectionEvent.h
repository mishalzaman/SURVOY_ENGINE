#pragma once

#include <glm/glm.hpp>
#include "Event.h"

class CameraViewProjectionEvent : public Event
{
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

public:
    CameraViewProjectionEvent(const glm::mat4& view, const glm::mat4& projection)
        : viewMatrix(view), projectionMatrix(projection) {}

    const glm::mat4& getViewMatrix() const { return viewMatrix; }
    const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
};

