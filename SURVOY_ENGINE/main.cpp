
/*
		  *--------------------------*
		     Current version: 0.1.16
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
		 [ ] - Clean up header includes
		 [x] - Clean up the Core class
		 [x] - Remove BAE namespace
		 [ ] - Add profiler
		 [ ] - Add shadow map
		 [ ] - Add SSAO


*/

#include <memory>
#include <string>
#include "Core.h"
#include "Scene0.h"

int main(int argc, char* args[]) {
	auto Core = std::make_unique<ENGINE::Core>();
	if (!Core->CreateDevice("Automata 0.1.15")) { return Core->GetError(); }

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
					break;
				case SDL_MOUSEMOTION:
					scene0->GetEventManager().notifyAll(InputMouseRelXYEvent(e.motion.xrel, e.motion.yrel));
					break;
				default:
					break;
			}
		}
	
		scene0->UpdatePrePhysics();
	
		/*=============
		FIXED UPDATE
		=============*/
		while (Core->Timer->PhysicsUpdate()) {
			scene0->UpdatePhysics(deltaTime);
		}
	
		scene0->UpdatePostPhysics();
	
		/*=============
		RENDER
		=============*/
		Core->BeginRender();
			
		glViewport(0, 0, ENGINE::Defaults::BASE_SCREEN_WIDTH, ENGINE::Defaults::BASE_SCREEN_HEIGHT);
	
		scene0->Render();
	
		//std::cout << Core->Timer->DeltaTimeS() << std::endl;
	
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
