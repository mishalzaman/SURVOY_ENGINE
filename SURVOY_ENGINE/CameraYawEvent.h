#pragma once

#include "Event.h"

class CameraYawEvent : public Event
{
    float Yaw;

public:
    CameraYawEvent(const float& yaw)
        : Yaw(yaw) {}

    const float& getYaw() const { return Yaw; }
};
