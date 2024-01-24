
/*
DEVELOPMENT NOTES
=================

BLENDER -> OpenGL
-----------------

Exporting:
    - Select all meshes to export
    - Export to FBX
    - Triangulate faces
    - Set scale to 0.01?

*/


#include <memory>
#include <iostream>
#include "Core.h"
#include "RendererText2D.h"
#include "Defaults.h"
#include "Camera2D.h"
#include "Model.h"
#include "Shader.h"
#include "Physics.h"
#include "PhysicsDebugDraw.h"
#include "Grid.h"
#include "CameraFreeLook.h"
#include "CameraFollow.h"
#include "CharacterController.h"

const float SCREEN_WIDTH = BAE::Defaults::BASE_SCREEN_WIDTH;
const float SCREEN_HEIGHT = BAE::Defaults::BASE_SCREEN_HEIGHT;

int main(int argc, char* args[]) {
	/*============= 
	ENGINE / CORE
	=============*/

	auto Core = std::make_unique<BAE::Core>();
	if (!Core->CreateDevice("Automata 0.2.0")) { return Core->GetError();}

	/*============= 
	INITIALIZATIONS
	=============*/

	/*-------------
	Helper
	--------------*/
	auto Grid = std::make_unique<BAE::Grid>();

	/*-------------
	3D
	--------------*/
	auto LevelModel = std::make_unique<BAE::Model>("assets/TestLevel/TestLevel.fbx");
	auto CameraFollow= std::make_unique<BAE::CameraFollow>(SCREEN_WIDTH, SCREEN_HEIGHT);
	auto CameraFreeLook = std::make_unique<BAE::CameraFreeLook>(SCREEN_WIDTH, SCREEN_HEIGHT);
	auto Shader3D = std::make_unique<BAE::Shader>("lighting_3d_vertex.glsl", "lighting_3d_fragment.glsl");



	/*-------------
	3D - Physics
	--------------*/
	auto Physics = std::make_unique<BAE::Physics>();
	auto CharacterController = std::make_unique<BAE::CharacterController>(Physics->World(), Physics->CollisionShapes());

	for (int i = 0; i < LevelModel->Meshes().size(); i++) {
		if (LevelModel->Meshes()[i].Name() == "PLAYER_START") {
			CharacterController->CreatePhysicalCharacter(
				LevelModel->Meshes()[i].Position()
			);

			continue;
		}
		
		Physics->StaticTriangleMesh(LevelModel->Meshes()[i].Vertices(), LevelModel->Meshes()[i].TransformationMat4());
	}

	/*-------------
	2D
	--------------*/
	auto Camera2D = std::make_unique<BAE::Camera2D>(SCREEN_WIDTH, SCREEN_HEIGHT);
	auto FontTexture = std::make_unique<BAE::STexture>();
		FontTexture->path = "assets/ExportedFont.bmp";
		BAE::FileLoader::Texture(*FontTexture);
	auto Shader2D = std::make_unique<BAE::Shader>("vertex_2d.glsl", "fragment_2d.glsl");

	/*=============
	SETUP
	=============*/

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
				// handle mouse motion
			case SDL_MOUSEMOTION:
				CameraFollow->SetMouseOffsets(
					e.motion.xrel,
					e.motion.yrel
				);
				CameraFreeLook->SetMouseOffsets(
					e.motion.xrel,
					e.motion.yrel
				);
				break;
			default:
				break;
			}
		}
		
		while (Core->Timer->PhysicsUpdate()) {
			CharacterController->UpdateYaw(CameraFollow->Yaw());
			CharacterController->Move(deltaTime);

			Physics->Simulate(deltaTime);

			CameraFollow->Target(CharacterController->Position());
			CameraFollow->Orbit(deltaTime);
			CameraFollow->Move();

			CameraFreeLook->Update(deltaTime);
		}

		/*=============
		RENDER
		=============*/
		Core->BeginRender();

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		/*=============
		RENDER - 3D
		=============*/

		Physics->DrawDebug(CameraFollow->Projection(), CameraFollow->View());

		Grid->render(CameraFollow->Projection(), CameraFollow->View(), glm::vec3(0, 0, 0));

		Shader3D->use();
		Shader3D->setMat4("projection", CameraFollow->Projection());
		Shader3D->setMat4("view", CameraFollow->View());
		Shader3D->setVec3("lightPos", glm::vec3(8, 10, 8));
		Shader3D->setVec3("viewPos", CameraFollow->Position());
		Shader3D->setVec3("lightColor", glm::vec3(1, 1, 1));

		LevelModel->Draw(*Shader3D);



		//Physics->DrawDebug(CameraFreeLook->Projection(), CameraFreeLook->View());

		//Grid->render(CameraFreeLook->Projection(), CameraFreeLook->View(), glm::vec3(0, 0, 0));

		//Shader3D->use();
		//Shader3D->setMat4("projection", CameraFreeLook->Projection());
		//Shader3D->setMat4("view", CameraFreeLook->View());
		//Shader3D->setVec3("lightPos", glm::vec3(8, 10, 8));
		//Shader3D->setVec3("viewPos", CameraFreeLook->Position());
		//Shader3D->setVec3("lightColor", glm::vec3(1, 1, 1));

		//LevelModel->Draw(*Shader3D);

		/*=============
		RENDER - 2D
		=============*/

		Shader2D->use();
        glm::mat4 projectionMatrix = Camera2D->Mat4Projection();
		Shader2D->setMat4("projection", projectionMatrix);
		Shader2D->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));

        BAE::RendererText2D::Render(
			Shader2D->ID,
			FontTexture->id,
            std::to_string(Core->Timer->DeltaTimeMS()) + " ms",
            0,
            16,
            glm::vec3(1, 1, 1),
            1
        );


		//BAE::RendererText2D::Render(
		//	Shader2D->ID,
		//	FontTexture->id,
		//	"Camera Position:\n" + std::to_string(CameraFreeLook->Position().x) + ",\n" + std::to_string(CameraFreeLook->Position().y) + ",\n" + std::to_string(CameraFreeLook->Position().z) + "",
		//	0,
		//	32,
		//	glm::vec3(1, 1, 1),
		//	1
		//);

		//BAE::RendererText2D::Render(
		//	Shader2D->ID,
		//	FontTexture->id,
		//	"Camera Forward:\n" + std::to_string(CameraFreeLook->Forward().x) + ",\n" + std::to_string(CameraFreeLook->Forward().y) + ",\n" + std::to_string(CameraFreeLook->Forward().z) + "",
		//	0,
		//	112,
		//	glm::vec3(1, 1, 1),
		//	1
		//);

		BAE::RendererText2D::Render(
			Shader2D->ID,
			FontTexture->id,
			"Character Position:\n" + std::to_string(CharacterController->Position().x) + ",\n" + std::to_string(CharacterController->Position().y) + ",\n" + std::to_string(CharacterController->Position().z) + "",
			0,
			208,
			glm::vec3(1, 1, 1),
			1
		);

		//BAE::RendererText2D::Render(
		//	Shader2D->ID,
		//	FontTexture->id,
		//	"Character Forward:\n" + std::to_string(CharacterController->Forward().x) + ",\n" + std::to_string(CharacterController->Forward().y) + ",\n" + std::to_string(CharacterController->Forward().z) + "",
		//	0,
		//	288,
		//	glm::vec3(1, 1, 1),
		//	1
		//);

		Core->EndRender();
	}

	/*=============
	SHUTDOWN
	=============*/

	Core->DestroyDevice();

	return BAE::Code::CORE_SUCCESS;

	//auto core = std::make_unique<BAE::Core>();

	//if (
	//	!core->CreateDevice("Automata 0.2.0")
	//	) {
	//	return core->GetError();
	//}

 //   /*============= 
 //   INITIALIZATIONS
 //   =============*/

 //   auto grid = std::make_unique<BAE::Grid>();

 //   /*=============
 //   3D
 //   *=============*/

 //   // model
 //   auto modelmesh = std::make_unique<BAE::Model>("assets/TestLevel/TestLevel.fbx");

 //   // Camera
 //   auto camera3d = std::make_unique<BAE::Camera3D>(glm::vec3(0), 1024.0f, 768.0f);

 //   // Shader
 //   auto shader3D = std::make_unique<BAE::Shader>("vertex_model_3d.glsl", "fragment_model_3d.glsl");

 //   // Physics Character
 //   auto character = std::make_unique<BAE::PhysicsCharacter>(glm::vec3(0,10,0));
 //   
 //   // Update camera
 //   camera3d->SetPosition(glm::vec3(0));
 //   camera3d->SetForward(character->ForwardV3());

 //   // Physics
 //   auto physics = std::make_unique<BAE::Physics>();
 //   //physics->CreateLevelGeometry(world->Vertices());
 //   physics->CreatePlayerGeometry(character->PositionV3(), character->YawF(), character->PitchF());

 //   for (unsigned int i = 0; i < modelmesh->Meshes().size(); i++) {
 //       physics->CreateLevelGeometry(modelmesh->Meshes()[i].Vertices(), modelmesh->Meshes()[i].TransformationMat4());
 //   }

 //   /**=============
 //   2D
 //   *=============*/
 //   auto camera2d = std::make_unique<BAE::Camera2D>(1024.0f, 768.0f);

 //   // FONT
 //   auto fontTexture = std::make_unique<BAE::STexture>();
 //   fontTexture->path = "assets/ExportedFont.bmp";
 //   BAE::FileLoader::Texture(*fontTexture);
 //   
 //   auto shader2D = std::make_unique<BAE::Shader>("vertex_2d.glsl", "fragment_2d.glsl");

 //   /*=============
 //   GAME LOOP
 //   =============*/
 //   bool quit = false;

 //   while (!core->Quit())
 //   {
 //       core->Timer->BeginFrame();

 //       while (core->Event->Poll()) {
 //           // Shutdown
 //           if (core->Event->EQuit()) { core->BeginShutdown(); }

 //           if (core->Event->Sym() == SDLK_ESCAPE) {
 //               core->BeginShutdown();
 //           }
 //       }

 //       character->Move(core->Timer->DeltaTimeMS());

 //       physics->UpdatePlayerGeometry(character->VelocityV3(), character->ForwardV3());

 //       while (core->Timer->PhysicsUpdate()) {
 //           /*=============
 //           PHYSICS
 //           =============*/
 //           physics->Simulate(core->Timer->DeltaTimeMS());
 //       }

 //       character->PositionV3(physics->PlayerPosition());

 //       camera3d->SetPosition(character->PositionV3());
 //       camera3d->SetForward(character->ForwardV3());
 //       camera3d->Update();

 //       /*=============
 //       RENDER
 //       =============*/

 //       core->BeginRender();

 //       //grid->render(camera3d->ProjectionMat4(), camera3d->ViewMat4(), glm::vec3(0, 0, 0));

 //       /*=============
 //       PHYSICS DEBUG
 //       =============*/

 //       //physics->DrawDebug(camera3d->ProjectionMat4(), camera3d->ViewMat4());

 //       /*=============
 //       3D
 //       =============*/

 //       glViewport(0, 0, 1024, 768);

 //       shader3D->use();

 //       shader3D->setMat4("projection", camera3d->ProjectionMat4());
 //       shader3D->setMat4("view", camera3d->ViewMat4());

 //       modelmesh->Draw(*shader3D);

 //       /*=============
 //       2D
 //       =============*/

 //       shader2D->use();
 //       glm::mat4 projectionMatrix = camera2d->Mat4Projection();
 //       shader2D->setMat4("projection", projectionMatrix);
 //       shader2D->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));

 //       BAE::RendererText2D::Render(
 //           shader2D->ID,
 //           fontTexture->id,
 //           std::to_string(core->Timer->DeltaTimeMS()) + " ms",
 //           0,
 //           0,
 //           glm::vec3(1, 1, 1),
 //           1
 //       );

 //       core->EndRender();
 //   }

 //   GLenum err;
 //   while ((err = glGetError()) != GL_NO_ERROR) {
 //       std::cerr << "OpenGL error: " << err << std::endl;
 //   }

	//core->DestroyDevice();

	//return BAE::Code::CORE_SUCCESS;
}
