#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <map>
#include "Defaults.h"
#include "Code.h"
#include "Scene.h"
#include "ShaderLibrary.h"
#include "TextureLibrary.h"
#include "Timer.h"
#include "Event.h"

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
		void BeginShutdown() { _quit = true; }

		void ResizeViewport(const int nWidth, const int nHeight);

		// Getters
		int GetError() { return _error; };

	private:
		const char* _title;

		SDL_Window* _window;
		SDL_GLContext _context;

		int _error;
		bool _quit;

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
	SHADER
	==============================================*/
	public:
		std::unique_ptr<ShaderLibrary> ShaderLibrary;
	/*==============================================
	TEXTURE
	==============================================*/
	public:
		std::unique_ptr<TextureLibrary> TextureLibrary;
	/*==============================================
	TIMER
	==============================================*/
	public:
		std::unique_ptr<Timer> Timer;
	/*==============================================
	SCENE
	==============================================*/
	public:
		std::unique_ptr<Scene> Scene;

	/*==============================================
	EVENT
	==============================================*/
	public:
		std::unique_ptr<Event> Event;
	};

}
