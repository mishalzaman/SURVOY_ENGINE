#include "Event.h"
#include <iostream>

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

Uint32 BAE::Event::Sym()
{
	return _event.key.keysym.sym;
}

Uint32 BAE::Event::_windowEvent()
{
	return _event.window.event;
}

bool BAE::Event::isBackSpace()
{
	return _event.key.keysym.sym == SDLK_BACKSPACE;
}

bool BAE::Event::HasWindowResized(int& newWidth, int& newHeight)
{
	if (_windowEvent() == SDL_WINDOWEVENT_RESIZED) {
		newWidth = _event.window.data1;
		newHeight = _event.window.data2;

		return true;
	}

	return false;
}
