#pragma once

#include <SDL2/SDL.h>
#include <string>

class Event
{
public:
	int Poll();

	Uint32 Type();
	std::string Text();

private:
	SDL_Event _event;
};