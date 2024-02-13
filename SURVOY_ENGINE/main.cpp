
/*
VERSION 0.1.5

DEVELOPMENT NOTES
=================

BLENDER -> OpenGL
-----------------

Exporting:
    - Select all meshes to export
    - Export to FBX
    - Triangulate faces
    - Set scale to 0.01

Features
	v 0.1.x
	=======
	- Player movement
	- Collision detection
	- Fire projectiles
	- Art direction
	- Player sounds

	Implementations
	---------------
	v 0.1.1
	- [x] Create Scene Base class
	- [x] Create Scene0 class
	- [x] Move all contents from main to Scene0 class
	v 0.1.4
	- [ ] Create test level
	v 0.1.5
	- [ ] Create ship movement
	- [ ] Create camere spline follow
	v 0.1.6
	- [ ] Work on art direction
	v 0.1.8
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
	
		std::cout << Core->Timer->DeltaTimeS() << std::endl;
	
		Core->EndRender();
	}
	
	/*=============
	SHUT DOWN
	=============*/

	scene0->Unload();

	Core->DestroyDevice();
	
	return 0;
}
