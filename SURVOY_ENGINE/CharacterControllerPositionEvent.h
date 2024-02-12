#pragma once

#include "Event.h"
#include <glm/glm.hpp>

class CharacterControllerPositionEvent : public Event
{
    glm::vec3 _position;

public:
    CharacterControllerPositionEvent(const glm::vec3& position)
        : _position(position) {}

    const glm::vec3& getPosition() const { return _position; }
};

