#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <map>
#include "Code.h"
#include "Scene.h"

/*
ShaderList
TextureList
Material: Shader, Texture
ObjectQuad: position, scale, Material, rotation, animation
ObjectText: position, scale, Material, text, rotation
*/

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
