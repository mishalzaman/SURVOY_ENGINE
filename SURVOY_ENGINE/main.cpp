
#include <memory>
#include <string>
#include "Core.h"
#include "Scene0.h"
#include "KeyPressEvent.h"

std::string version = "Basic Add Engine 0.1.48";

/*
		  *--------------------------*
		     Current version: 0.1.48
		  *--------------------------*

DEVELOPMENT NOTES
==================================================

BLENDER -> OpenGL
-----------------

Exporting:
    - Select all meshes to export
    - Export to FBX
    - Triangulate faces
    - Set scale to 0.01

DEVELOPMENT
===================================================
  0.1 Implementations
	The main goals of 0.1 is as follows:
	- to establish a stable system that can load FBX/OBJ model files
	  without errors
	- Consistent architecture
	- Establish the art direction

	Change Log
	---------------
	v 0.1.1
	- [x] Create Scene Base class
	- [x] Create Scene0 class
	- [x] Move all contents from main to Scene0 class
	v 0.1.4
	- [x] Create test level
	v 0.1.5
	- [x] Add framebuffer
	- [x] Add cubemap
	v 0.1.6
	- [x] Add back free look camera
	v 0.1.7
	- [x] Update lighting (gamma, hdr, ssao, blur)
	v 0.1.8+
	- [ ] Refactor
		 [x] - Fix model loading
			[-] - Allow models with no textures, but materials
			[x] - Fix issue with imports loading incorrect models
			[x] - Improve the mesh class
		 [x] - Clean up the Core class
		 [x] - Remove BAE namespace
		 [x] - Add profiler, better error handling
		 [x] - Update graphics pipeline
				- Updated System abstract class to ISystem with derived System
				- Updated System classes and removed redundant methods
				- Updated the update methods naming
				- Added phyics debug draw system and key input toggle
		 [x] - Add shadow map
		 [x] - Refactor shadow map
		 [ ] - Clean up graphics pipeline
		 [ ] - Add HDR
		 [ ] - Add deffered rendering
		 [ ] - Add SSAO


*/

int main(int argc, char* args[]) {
	auto Core = std::make_unique<ENGINE::Core>();
	if (!Core->CreateDevice(version.c_str())) { return Core->GetError(); }

	auto scene0 = std::make_unique<Scene0>();

	scene0->Load();

	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	/*=============
	LOOP
	=============*/
	
	SDL_Event e;

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		
	while (!Core->Quit())
	{
		Core->Timer->BeginFrame();
			
		float deltaTime = Core->Timer->DeltaTimeS();
			
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				// exit if the window is closed
				case SDL_QUIT:
					Core->BeginShutdown();
					break;
					// check for keypresses
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						Core->BeginShutdown();
					}
					if (e.key.keysym.sym == SDLK_z) {
						scene0->GetEventManager().notifyAll(KeyPressEvent(SDL_KeyCode::SDLK_z));
					}

					break;
				case SDL_MOUSEMOTION:
					scene0->GetEventManager().notifyAll(InputMouseRelXYEvent(e.motion.xrel, e.motion.yrel));
					break;
				default:
					break;
			}
		}
	
		/*=============
		FIXED UPDATE
		=============*/
		while (Core->Timer->FixedUpdate()) {
			scene0->UpdateOnFixedTimestep(deltaTime);
		}
	
		scene0->UpdateOnVariableTimestep();
	
		/*=============
		RENDER
		=============*/
		Core->BeginRender();
			
		glViewport(0, 0, ENGINE::Defaults::BASE_SCREEN_WIDTH, ENGINE::Defaults::BASE_SCREEN_HEIGHT);
	
		scene0->Render();
	
		std::cout << Core->Timer->DeltaTimeS() << std::endl;
	
		Core->EndRender();
	}

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (err)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default:                               error = "UNKNOWN_ERROR"; break;
		}
		std::cerr << "GL_" << error << std::endl;
	}
	
	/*=============
	SHUT DOWN
	=============*/

	scene0->Unload();

	Core->DestroyDevice();
	
	return 0;
}
