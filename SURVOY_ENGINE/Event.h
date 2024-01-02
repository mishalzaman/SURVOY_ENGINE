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

		bool HasWindowResized(int& newWidth, int& newHeight);
 
		// Events
		bool EQuit() { return _event.type == SDL_QUIT; }
		bool EKeyDown() { return _event.type == SDL_KEYDOWN; }
		bool ETextInput() { return _event.type == SDL_TEXTINPUT; }
		bool EWindowEvent() { return _event.type == SDL_WINDOWEVENT; }

		SDL_Event& GetEvent() { return _event; }
		Uint32 Sym();
	private:
		SDL_Event _event;

		Uint32 _windowEvent();
	};
}
