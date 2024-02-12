
/*
VERSION 0.1.0

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
#include "ECSCommon.h"

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

	// Mesh
	for (int i = 0; i < LevelModel->Meshes().size(); i++) {
		if (LevelModel->Meshes()[i].Name() == "PLAYER_START") {			
			entityId = entityManager->createEntity();
			entityManager->addComponent<ECS::DynamicCapsulePhysicsBodyComponent>(entityId);
			entityManager->addComponent<ECS::VelocityComponent>(entityId);
			entityManager->addComponent<ECS::OrientationComponent>(entityId, LevelModel->Meshes()[i].Position());
			entityManager->addByTag("PlayerController", entityId);
		}
		else {
			entityId = entityManager->createEntity();

			entityManager->addComponent<ECS::TransformComponent>(entityId, LevelModel->Meshes()[i].Transformation());
			entityManager->addComponent<ECS::MeshComponent>(
				entityId,
				LevelModel->Meshes()[i].Name(),
				LevelModel->Meshes()[i].Vertices(),
				LevelModel->Meshes()[i].Indices()
			);
			entityManager->addComponent<ECS::BuffersComponent>(entityId);
			entityManager->addComponent<ECS::TexturesComponent>(entityId, LevelModel->Meshes()[i].Textures());
			entityManager->addComponent<ECS::StaticPhysicsBodyComponent>(entityId);
			entityManager->addComponent<ECS::CameraMatricesComponent>(entityId);

			entityManager->addByTag("Mesh", entityId);
		}
	}

	// CameraThirdPerson
	entityId = entityManager->createEntity();
	entityManager->addComponent<ECS::ScreenDimensionsComponent>(
		entityId,
		1024.f,
		768.f
	);
	entityManager->addComponent<ECS::CameraMatricesComponent>(entityId);
	entityManager->addComponent<ECS::OrientationComponent>(entityId, glm::vec3(0, 1, 0));
	entityManager->addComponent<ECS::CameraMouseComponent>(entityId);
	entityManager->addComponent<ECS::TargetComponent>(entityId);
	entityManager->addByTag("CameraThirdPerson", entityId);

	// Shader
	entityManager->addComponent<ECS::ProgramComponent>(entityId, *shader3d);
	entityManager->addByTag("Shader 3d", entityId);

	/*=============
	LOAD
	=============*/

	// Pass a reference to the EntityManager object
	auto systemManager = std::make_unique<ECS::SystemManager>();

	//systemManager->AddSystem<ECS::CameraFreeLookSystem>(*entityManager, *physics, *eventManager);
	systemManager->AddSystem<ECS::CharacterControllerSystem>(*entityManager, *physics, *eventManager);
	systemManager->AddSystem<ECS::PhysicsSystem>(*entityManager, *physics, *eventManager);
	systemManager->AddSystem<ECS::CameraThirdPersonSystem>(*entityManager, *physics, *eventManager);
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

		systemManager->UpdatePrePhysics();

		/*=============
		FIXED UPDATE
		=============*/
		while (Core->Timer->PhysicsUpdate()) {
			systemManager->Update(deltaTime);
		}

		systemManager->UpdatePostPhysics();

		/*=============
		RENDER
		=============*/
		Core->BeginRender();
		
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		systemManager->Render();

		std::cout << Core->Timer->DeltaTimeS() << std::endl;

		Core->EndRender();
	}

	/*=============
	SHUT DOWN
	=============*/

	systemManager->Unload();

	Core->DestroyDevice();

	return 0;
}
