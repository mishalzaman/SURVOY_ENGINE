
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Core.h"
#include "RendererText2D.h"
#include "Defaults.h"
#include "Camera3D.h"
#include "Camera2D.h"
#include "Model.h"
#include "Shader.h"
#include "STexture.h"
#include "FileLoader.h"
#include "World.h"
#include "Renderer3D.h"
#include "Physics.h"
#include "PhysicsDebugDraw.h"
#include "Grid.h"
#include "PhysicsCharacter.h"

int main(int argc, char* args[]) {
	auto core = std::make_unique<BAE::Core>();

	if (
		!core->CreateDevice("Automata 0.2.0")
		) {
		return core->GetError();
	}

    /*============= 
    INITIALIZATIONS
    =============*/

    auto grid = std::make_unique<BAE::Grid>();

    /*=============
    3D
    *=============*/

    auto modelmesh = std::make_unique<BAE::Model>("assets/Level0/block_a_b.fbx");

    // Camera
    auto camera3d = std::make_unique<BAE::Camera3D>(glm::vec3(0), 1024.0f, 768.0f);

    // Shader
    auto shader3D = std::make_unique<BAE::Shader>("vertex_model_3d.glsl", "fragment_model_3d.glsl");

    // World Mesh
    auto imageTexture = std::make_unique<BAE::STexture>();
    imageTexture->path = "assets/tilemap/tilesheet.png";
    BAE::FileLoader::Texture(*imageTexture);

    std::vector<int> map = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1,
        1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    auto world = std::make_unique<BAE::World>(map, 16);

    // Renderer
    auto renderer3d = std::make_unique<BAE::Renderer3D>(imageTexture->id, world->Vertices());

    // Physics Character
    auto character = std::make_unique<BAE::PhysicsCharacter>(world->CameraPosition());
    
    // Update camera
    camera3d->SetPosition(character->PositionV3());
    camera3d->SetForward(character->ForwardV3());

    // Physics
    auto physics = std::make_unique<BAE::Physics>();
    physics->CreateLevelGeometry(world->Vertices());
    physics->CreatePlayerGeometry(character->PositionV3(), character->YawF(), character->PitchF());

    /**=============
    2D
    *=============*/
    auto camera2d = std::make_unique<BAE::Camera2D>(1024.0f, 768.0f);

    // FONT
    auto fontTexture = std::make_unique<BAE::STexture>();
    fontTexture->path = "assets/ExportedFont.bmp";
    BAE::FileLoader::Texture(*fontTexture);
    
    auto shader2D = std::make_unique<BAE::Shader>("vertex_2d.glsl", "fragment_2d.glsl");

    /*=============
    GAME LOOP
    =============*/
    bool quit = false;

    while (!core->Quit())
    {
        core->Timer->BeginFrame();

        while (core->Event->Poll()) {
            // Shutdown
            if (core->Event->EQuit()) { core->BeginShutdown(); }

            if (core->Event->Sym() == SDLK_ESCAPE) {
                core->BeginShutdown();
            }
        }

        character->Move(core->Timer->DeltaTimeMS());

        physics->UpdatePlayerGeometry(character->VelocityV3(), character->ForwardV3());

        while (core->Timer->PhysicsUpdate()) {
            /*=============
            PHYSICS
            =============*/
            physics->Simulate(core->Timer->DeltaTimeMS());
        }

        //character->PositionV3(physics->PlayerPosition());

        camera3d->SetPosition(character->PositionV3());
        camera3d->SetForward(character->ForwardV3());
        camera3d->Update();

        /*=============
        RENDER
        =============*/

        core->BeginRender();

        //grid->render(camera3d->ProjectionMat4(), camera3d->ViewMat4(), glm::vec3(0, 0, 0));

        /*=============
        PHYSICS DEBUG
        =============*/

        //physics->DrawDebug(camera3d->ProjectionMat4(), camera3d->ViewMat4());

        /*=============
        3D
        =============*/

        glViewport(0, 0, 1024, 768);

        shader3D->use();

        shader3D->setMat4("projection", camera3d->ProjectionMat4());
        shader3D->setMat4("view", camera3d->ViewMat4());

        //renderer3d->render(*shader3D, *camera3d, glm::vec3(0, 0, 0));

        modelmesh->Draw(*shader3D);

        /*=============
        2D
        =============*/

        shader2D->use();
        glm::mat4 projectionMatrix = camera2d->Mat4Projection();
        shader2D->setMat4("projection", projectionMatrix);
        shader2D->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));

        BAE::RendererText2D::Render(
            shader2D->ID,
            fontTexture->id,
            std::to_string(core->Timer->DeltaTimeMS()) + " ms",
            0,
            0,
            glm::vec3(1, 1, 1),
            1
        );

        core->EndRender();
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

	core->DestroyDevice();

	return BAE::Code::CORE_SUCCESS;
}
