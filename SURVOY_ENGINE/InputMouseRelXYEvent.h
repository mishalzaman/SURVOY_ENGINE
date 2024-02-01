#pragma once
#include "Event.h"
class InputMouseRelXYEvent : public Event
{
    float _mouseX;
    float _mouseY;

public:
    InputMouseRelXYEvent(const float& mouseX, const float& mouseY)
        : _mouseX(mouseX), _mouseY(mouseY) {}

    const float& getMouseX() const { return _mouseX; }
    const float& getMouseY() const { return _mouseY; }
};

