#include "Event.h"

int Event::Poll()
{
	return SDL_PollEvent(&_event);
}

Uint32 Event::Type()
{
	return _event.type;
}

std::string Event::Text()
{
	return _event.text.text;
}
