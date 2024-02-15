#pragma once

#include "Event.h"

class FrameBufferColourBufferEvent : public Event
{
    unsigned int ColourBuffer;

public:
    FrameBufferColourBufferEvent(const unsigned int& colourBuffer)
        : ColourBuffer(colourBuffer) {}

    const unsigned int& getColourBuffer() const { return ColourBuffer; }
};
