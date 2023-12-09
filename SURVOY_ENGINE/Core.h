#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <map>
#include "Code.h"
#include "Scene.h"
#include "ShaderLibrary.h"
#include "TextureLibrary.h"
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
		bool CreateDevice(
			int width,
			int height,
			const char* title);

		void DestroyDevice();
		void BeginRender();
		void EndRender();

		// Getters
		int GetError() { return _error; };

	private:
		float _screenW;
		float _screenH;
		const char* _title;

		SDL_Window* _window;
		SDL_GLContext _context;

		int _error;

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
	Logger
	==============================================*/
	//public:
	//	std::unique_ptr<Logger> Logger;
	};

}
