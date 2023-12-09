#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace BAE {
	class Event
	{
	public:
		int Poll();

		Uint32 Type();
		std::string Text();

		bool isBackSpace();

	private:
		SDL_Event _event;
	};
}
