#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <map>
#include "Defaults.h"
#include "Code.h"
#include "Timer.h"

namespace BAE {
	class Core
	{
	/*==============================================
	CORE
	==============================================*/
	public:
		Core();
		~Core();
		bool CreateDevice(const char* title);

		void DestroyDevice();
		void BeginRender();
		void EndRender();
		bool Quit() { return _quit; }
		void BeginShutdown();

		// Getters
		int GetError() { return _error; };

	private:
		const char* _title;

		SDL_Window* _window;
		SDL_GLContext _context;

		int _error;
		bool _quit;

	/*==============================================
	WINDOW
	==============================================*/
		void _resizeViewport(const int nWidth, const int nHeight);

	/*==============================================
	INITIALIZATIONS
	==============================================*/
	private:
		bool _initSDL();
		bool _initOpengGL();
		bool _createWindow();
		bool _createContext();
		bool _initGlew();
		void _openGLSettings();
		void _initializeSubSystems();

	/*==============================================
	TIMER
	==============================================*/
	public:
		std::unique_ptr<Timer> Timer;
	};

}
