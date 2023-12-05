#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <map>
#include "Code.h"
#include "Scene.h"

namespace ENGINE {
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
		void StartDevice();
		void DestroyDevice();
		void BeginScene();
		void EndScene();

		// Getters
		int GetError() { return _error; };

	private:
		float _screenW;
		float _screenH;

		SDL_Window* _window;
		SDL_GLContext _context;

		int _error;

	/*==============================================
	FONT
	==============================================*/

	/*==============================================
	RENDERER
	==============================================*/

	/*==============================================
	EVENT
	==============================================*/

	/*==============================================
	SCENE
	==============================================*/
	public:
		std::unique_ptr<Scene> Scene;

	/*==============================================
	RESOURCE_MANAGER
	==============================================*/
	};

}
