#pragma once

#include "Event.h"

class MoveDirectionEvent : public Event
{
public:
    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        TURN_LEFT,
        TURN_RIGHT
    };

    MoveDirectionEvent(const MoveDirectionEvent::Direction& direction)
        : _direction(direction) {}

    const float& getDirection() const { return _direction; }

private:
    Direction _direction;
};

#pragma once
