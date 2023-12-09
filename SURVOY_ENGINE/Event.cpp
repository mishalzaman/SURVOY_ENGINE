#include "Event.h"

int BAE::Event::Poll()
{
	return SDL_PollEvent(&_event);
}

Uint32 BAE::Event::Type()
{
	return _event.type;
}

std::string BAE::Event::Text()
{
	return _event.text.text;
}

bool BAE::Event::isBackSpace()
{
	return _event.key.keysym.sym == SDLK_BACKSPACE;
}
