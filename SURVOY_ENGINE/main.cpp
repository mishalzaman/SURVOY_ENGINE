
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

ECS:
Core ECS Classes:

	Entity: Represents a unique identifier for game entities.
	Component: The base class for all components. Specific components inherit from this.
	System: The base class for all systems. Specific systems inherit from this.
	EntityManager: Manages entities and their associated components.
	SystemManager: Manages the update cycle and coordination of systems.
	ComponentManager: (Optional) Manages storage and retrieval of components.
	Specific Component Classes:

	TransformComponent: Stores position, rotation, scale.
	MeshComponent: Contains 3D model data for rendering.
	RigidBodyComponent: Holds physics properties for dynamics simulation.
	StaticBodyComponent: Used for static (non-moving) physics objects.
	CameraComponent: Holds data for camera view, projection.
	LightComponent: Data for different types of lights.
	InputComponent: Stores player input state.
	AIComponent: For AI-controlled entities.
	HealthComponent: To keep track of health or damage state.
	AnimationComponent: For animated entities.
	AudioComponent: For sound effects and music.
	Specific System Classes:

	RenderSystem: Handles rendering of entities with Mesh and Transform components.
	PhysicsSystem: Manages physical interactions and updates RigidBody components.
	InputSystem: Processes player inputs and updates relevant components.
	AISystem: Controls AI behavior.
	AnimationSystem: Manages animations for entities.
	AudioSystem: Handles playback and manipulation of audio.
	CollisionSystem: Detects and responds to collisions between entities.
	Utility Classes:

	Vector3, Quaternion, Matrix: For mathematical operations.
	ResourceManager: Manages loading and accessing resources like textures, models, audio files.
	SceneGraph: (Optional) Manages hierarchical relationships between entities.
	EventDispatcher: For handling and dispatching events.
	Timer: For managing game time and timers.
	Gameplay-Specific Classes:

	CharacterController: For player character movement logic.
	EnemyController: For enemy behavior.
	Weapon: For handling weapon logic.
	Inventory: For managing player inventory.
	QuestManager: For quest tracking and updates.
	Networking (if multiplayer):

	UIManager: Manages UI elements and interactions.
	Button, Slider, TextField: Specific UI element classes.
	Debugging and Tools:

	DebugDrawer: For rendering debug information.
	Profiler: For performance profiling.
	Editor: (Optional) For in-game level editing or adjustments.
*/

#include <memory>
#include "Core.h"
#include "Model.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "BuffersComponent.h"
#include "TexturesComponent.h"
#include "MeshRenderSystem.h"
#include "CameraFreeLookSystem.h"
#include "CameraComponent.h"
#include "MouseRelXY.h"
#include "Physics.h"
#include "StaticPhysicsBodyComponent.h"
#include "PhysicsSystem.h"
#include "EventManager.h"
#include "InputMouseRelXYEvent.h"
#include "ScreenDimensionsComponent.h"
#include "CameraMatricesComponent.h"
#include "CameraMouseComponent.h"
#include "CameraOrientationComponent.h"
#include "ProgramComponent.h"

const float SCREEN_WIDTH = BAE::Defaults::BASE_SCREEN_WIDTH;
const float SCREEN_HEIGHT = BAE::Defaults::BASE_SCREEN_HEIGHT;

int main(int argc, char* args[]) {

	/*=============
	ENGINE / CORE
	=============*/

	auto Core = std::make_unique<BAE::Core>();
	if (!Core->CreateDevice("Automata 0.2.0")) { return Core->GetError(); }

	auto shader3d = std::make_unique<Shader>("lighting_3d_vertex.glsl", "lighting_3d_fragment.glsl");

	/*=============
	OBSERVER
	=============*/
	auto eventManager = std::make_unique<EventManager>();

	/*=============
	PHYSICS
	=============*/
	auto physics = std::make_unique<BAE::Physics>();

	/*=============
	ECS SETUP
	=============*/
	auto entityManager = std::make_unique<ECS::EntityManager>();

	/*=============
	INITIALIZE
	=============*/

	auto LevelModel = std::make_unique<BAE::Model>("assets/Level_0/level.fbx");

	int entityId = 0;

	for (int i = 0; i < LevelModel->Meshes().size(); i++) {
		// Create a new entity for each mesh
		entityId = entityManager->createEntity();

		entityManager->addComponent<ECS::TransformComponent>(
			entityId,
			LevelModel->Meshes()[i].Position(), // Position
			glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // Rotation
			glm::vec3(1.0f), // Scale
			LevelModel->Meshes()[i].Transformation()  // Transformation matrix (identity matrix as an example)
		);

		entityManager->addComponent<ECS::MeshComponent>(
			entityId,
			LevelModel->Meshes()[i].Name(),
			LevelModel->Meshes()[i].Vertices(),
			LevelModel->Meshes()[i].Indices()
		);

		entityManager->addComponent<ECS::BuffersComponent>(entityId);

		entityManager->addComponent<ECS::TexturesComponent>(
			entityId,
			LevelModel->Meshes()[i].Textures()
		);
		entityManager->addComponent<ECS::StaticPhysicsBodyComponent>(entityId);
		entityManager->addComponent<ECS::CameraMatricesComponent>(entityId);
	}

	// Camera
	int cameraEntityId = entityManager->createEntity();
	entityManager->addComponent<ECS::ScreenDimensionsComponent>(
		cameraEntityId,
		1024.f,
		768.f
	);
	entityManager->addComponent<ECS::CameraMatricesComponent>(cameraEntityId);
	entityManager->addComponent<ECS::CameraOrientationComponent>(cameraEntityId, glm::vec3(0, 1, 0));
	entityManager->addComponent<ECS::CameraMouseComponent>(cameraEntityId);


	/*=============
	LOAD
	=============*/

	// Pass a reference to the EntityManager object
	auto systemManager = std::make_unique<ECS::SystemManager>();

	systemManager->AddSystem<ECS::CameraFreeLookSystem>(*entityManager, *physics, *eventManager, cameraEntityId);
	systemManager->AddSystem<ECS::PhysicsSystem>(*entityManager, *physics);
	systemManager->AddSystem<ECS::MeshRenderSystem>(*entityManager, *physics, *eventManager);

	systemManager->Load();

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
					//systemManager->UpdateVec3(e.motion.xrel, e.motion.yrel, 0.f);
					eventManager->notifyAll(InputMouseRelXYEvent(e.motion.xrel, e.motion.yrel));
					break;
				default:
					break;
			}
		}

		systemManager->Update();

		/*=============
		FIXED UPDATE
		=============*/
		while (Core->Timer->PhysicsUpdate()) {
			systemManager->Update(deltaTime);
		}

		/*=============
		RENDER
		=============*/
		Core->BeginRender();
		
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		
		//physics->DrawDebug(
		//	entityManager->getComponent<ECS::CameraComponent>(cameraEntityId)->Projection,
		//	entityManager->getComponent<ECS::CameraComponent>(cameraEntityId)->View
		//);

		systemManager->Renders();

		//std::cout << Core->Timer->DeltaTimeS() << std::endl;

		Core->EndRender();
	}

	/*=============
	SHUT DOWN
	=============*/

	systemManager->Unload();

	Core->DestroyDevice();

	return 0;
}

//#include <memory>
//#include <iostream>
//#include "Core.h"
//#include "RendererText2D.h"
//#include "Defaults.h"
//#include "Camera2D.h"
//#include "Model.h"
//#include "Shader.h"
//#include "Physics.h"
//#include "PhysicsDebugDraw.h"
//#include "Grid.h"
//#include "CameraFreeLook.h"
//#include "CameraFollow.h"
//#include "CharacterController.h"
//
//const float SCREEN_WIDTH = BAE::Defaults::BASE_SCREEN_WIDTH;
//const float SCREEN_HEIGHT = BAE::Defaults::BASE_SCREEN_HEIGHT;
//
//int main(int argc, char* args[]) {
//	/*============= 
//	ENGINE / CORE
//	=============*/
//
//	auto Core = std::make_unique<BAE::Core>();
//	if (!Core->CreateDevice("Automata 0.2.0")) { return Core->GetError();}
//
//	/*============= 
//	INITIALIZATIONS
//	=============*/
//
//	/*-------------
//	Helper
//	--------------*/
//	auto Grid = std::make_unique<BAE::Grid>();
//
//	/*-------------
//	3D
//	--------------*/
//	auto LevelModel = std::make_unique<BAE::Model>("assets/TestLevel/TestLevel.fbx");
//	auto CameraFollow= std::make_unique<BAE::CameraFollow>(SCREEN_WIDTH, SCREEN_HEIGHT);
//	auto CameraFreeLook = std::make_unique<BAE::CameraFreeLook>(SCREEN_WIDTH, SCREEN_HEIGHT);
//	auto Shader3D = std::make_unique<BAE::Shader>("lighting_3d_vertex.glsl", "lighting_3d_fragment.glsl");
//
//
//
//	/*-------------
//	3D - Physics
//	--------------*/
//	auto Physics = std::make_unique<BAE::Physics>();
//	auto CharacterController = std::make_unique<BAE::CharacterController>(Physics->World(), Physics->CollisionShapes());
//
//	for (int i = 0; i < LevelModel->Meshes().size(); i++) {
//		if (LevelModel->Meshes()[i].Name() == "PLAYER_START") {
//			CharacterController->CreatePhysicalCharacter(
//				LevelModel->Meshes()[i].Position()
//			);
//
//			continue;
//		}
//		
//		Physics->StaticTriangleMesh(LevelModel->Meshes()[i].Vertices(), LevelModel->Meshes()[i].TransformationMat4());
//	}
//
//	/*-------------
//	2D
//	--------------*/
//	auto Camera2D = std::make_unique<BAE::Camera2D>(SCREEN_WIDTH, SCREEN_HEIGHT);
//	auto FontTexture = std::make_unique<BAE::STexture>();
//		FontTexture->path = "assets/ExportedFont.bmp";
//		BAE::FileLoader::Texture(*FontTexture);
//	auto Shader2D = std::make_unique<BAE::Shader>("vertex_2d.glsl", "fragment_2d.glsl");
//
//	/*=============
//	SETUP
//	=============*/
//
//	SDL_SetRelativeMouseMode(SDL_TRUE);
//
//	/*=============
//	LOOP
//	=============*/
//
//	SDL_Event e;
//
//	while (!Core->Quit())
//	{
//		Core->Timer->BeginFrame();
//
//		float deltaTime = Core->Timer->DeltaTimeS();
//
//		while (SDL_PollEvent(&e) != 0) {
//			switch (e.type) {
//				// exit if the window is closed
//			case SDL_QUIT:
//				Core->BeginShutdown();
//				break;
//				// check for keypresses
//			case SDL_KEYDOWN:
//				if (e.key.keysym.sym == SDLK_ESCAPE) {
//					Core->BeginShutdown();
//				}
//				break;
//				// handle mouse motion
//			case SDL_MOUSEMOTION:
//				CameraFollow->SetMouseOffsets(
//					e.motion.xrel,
//					e.motion.yrel
//				);
//				CameraFreeLook->SetMouseOffsets(
//					e.motion.xrel,
//					e.motion.yrel
//				);
//				break;
//			default:
//				break;
//			}
//		}
//		
//		while (Core->Timer->PhysicsUpdate()) {
//			CharacterController->UpdateYaw(CameraFollow->Yaw());
//			CharacterController->Move(deltaTime);
//
//			Physics->Simulate(deltaTime);
//
//			CameraFollow->Target(CharacterController->Position());
//			CameraFollow->Orbit(deltaTime);
//			CameraFollow->Move();
//
//			CameraFreeLook->Update(deltaTime);
//		}
//
//		/*=============
//		RENDER
//		=============*/
//		Core->BeginRender();
//
//		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//
//		/*=============
//		RENDER - 3D
//		=============*/
//
//		Physics->DrawDebug(CameraFollow->Projection(), CameraFollow->View());
//
//		Grid->render(CameraFollow->Projection(), CameraFollow->View(), glm::vec3(0, 0, 0));
//
//		Shader3D->use();
//		Shader3D->setMat4("projection", CameraFollow->Projection());
//		Shader3D->setMat4("view", CameraFollow->View());
//		Shader3D->setVec3("lightPos", glm::vec3(8, 10, 8));
//		Shader3D->setVec3("viewPos", CameraFollow->Position());
//		Shader3D->setVec3("lightColor", glm::vec3(1, 1, 1));
//
//		LevelModel->Draw(*Shader3D);
//
//
//
//		//Physics->DrawDebug(CameraFreeLook->Projection(), CameraFreeLook->View());
//
//		//Grid->render(CameraFreeLook->Projection(), CameraFreeLook->View(), glm::vec3(0, 0, 0));
//
//		//Shader3D->use();
//		//Shader3D->setMat4("projection", CameraFreeLook->Projection());
//		//Shader3D->setMat4("view", CameraFreeLook->View());
//		//Shader3D->setVec3("lightPos", glm::vec3(8, 10, 8));
//		//Shader3D->setVec3("viewPos", CameraFreeLook->Position());
//		//Shader3D->setVec3("lightColor", glm::vec3(1, 1, 1));
//
//		//LevelModel->Draw(*Shader3D);
//
//		/*=============
//		RENDER - 2D
//		=============*/
//
//		Shader2D->use();
//        glm::mat4 projectionMatrix = Camera2D->Mat4Projection();
//		Shader2D->setMat4("projection", projectionMatrix);
//		Shader2D->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));
//
//        BAE::RendererText2D::Render(
//			Shader2D->ID,
//			FontTexture->id,
//            std::to_string(Core->Timer->DeltaTimeMS()) + " ms",
//            0,
//            16,
//            glm::vec3(1, 1, 1),
//            1
//        );
//
//
//		//BAE::RendererText2D::Render(
//		//	Shader2D->ID,
//		//	FontTexture->id,
//		//	"Camera Position:\n" + std::to_string(CameraFreeLook->Position().x) + ",\n" + std::to_string(CameraFreeLook->Position().y) + ",\n" + std::to_string(CameraFreeLook->Position().z) + "",
//		//	0,
//		//	32,
//		//	glm::vec3(1, 1, 1),
//		//	1
//		//);
//
//		//BAE::RendererText2D::Render(
//		//	Shader2D->ID,
//		//	FontTexture->id,
//		//	"Camera Forward:\n" + std::to_string(CameraFreeLook->Forward().x) + ",\n" + std::to_string(CameraFreeLook->Forward().y) + ",\n" + std::to_string(CameraFreeLook->Forward().z) + "",
//		//	0,
//		//	112,
//		//	glm::vec3(1, 1, 1),
//		//	1
//		//);
//
//		BAE::RendererText2D::Render(
//			Shader2D->ID,
//			FontTexture->id,
//			"Character Position:\n" + std::to_string(CharacterController->Position().x) + ",\n" + std::to_string(CharacterController->Position().y) + ",\n" + std::to_string(CharacterController->Position().z) + "",
//			0,
//			208,
//			glm::vec3(1, 1, 1),
//			1
//		);
//
//		//BAE::RendererText2D::Render(
//		//	Shader2D->ID,
//		//	FontTexture->id,
//		//	"Character Forward:\n" + std::to_string(CharacterController->Forward().x) + ",\n" + std::to_string(CharacterController->Forward().y) + ",\n" + std::to_string(CharacterController->Forward().z) + "",
//		//	0,
//		//	288,
//		//	glm::vec3(1, 1, 1),
//		//	1
//		//);
//
//		Core->EndRender();
//	}
//
//	/*=============
//	SHUTDOWN
//	=============*/
//
//	Core->DestroyDevice();
//
//	return BAE::Code::CORE_SUCCESS;
//}
