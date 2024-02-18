#pragma once

#include "Event.h"
#include <SDL2/SDL.h>

class KeyPressEvent : public Event
{
    SDL_KeyCode _key;

public:
    KeyPressEvent(const SDL_KeyCode& key)
        : _key(key) {}

    const float& getKey() const { return _key; }
};

