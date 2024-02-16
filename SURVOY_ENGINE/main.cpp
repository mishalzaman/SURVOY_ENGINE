
/*
VERSION 0.1.6

DEVELOPMENT NOTES
=================

BLENDER -> OpenGL
-----------------

Exporting:
    - Select all meshes to export
    - Export to FBX
    - Triangulate faces
    - Set scale to 0.01

Implementations
	v 0.1.x
	=======
	- Main theme of this iteration is to create a general sense
	  of the art direction
	- Art direction

	Implementations
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
	- [ ] Add back free look camera
	v 0.1.7
	- [ ] Fix lighting (gamma, hdr, ssao, blur)
	v 0.1.8
	- [ ] Add shadows
	v 0.1.9
	- [ ] Refactor

*/

#include <memory>
#include "Core.h"
#include "Scene0.h"

const float SCREEN_WIDTH = BAE::Defaults::BASE_SCREEN_WIDTH;
const float SCREEN_HEIGHT = BAE::Defaults::BASE_SCREEN_HEIGHT;

int main(int argc, char* args[]) {
	auto Core = std::make_unique<BAE::Core>();
	if (!Core->CreateDevice("Automata 0.2.0")) { return Core->GetError(); }

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
			
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
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
