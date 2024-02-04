#pragma once

#include <glm/glm.hpp>
#include "Event.h"

class CameraPositionEvent : public Event
{
    glm::vec3 Position;

public:
    CameraPositionEvent(const glm::vec3& Position)
        : Position(Position) {}

    const glm::vec3& getPosition() const { return Position; }
};
