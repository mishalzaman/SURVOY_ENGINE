
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Core.h"
#include "RendererQuad2D.h"
#include "RendererText2D.h"
#include "RendererTileset2D.h"
#include "MapLoader.h"
#include "Defaults.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "GridMeshGenerator.h"
#include "Defaults.h"
#include "Model.h"

bool attemptMove(int newRow, int newCol, std::vector<int> map, int mapWidth) {
    if (newRow >= 0 && newRow < map.size() / mapWidth && newCol >= 0 && newCol < mapWidth) {
        return true;
    }

    return false;
}

int main(int argc, char* args[]) {
	auto core = std::make_unique<BAE::Core>();

	if (
		!core->CreateDevice("Automata 0.2.0")
		) {
		return core->GetError();
	}

    /*-------------
    ASSETS
    -------------*/

    // 1024 x 768 2D shader
    core->ShaderLibrary->Add("shader_2d_1024_768", "vertex_2d.glsl", "fragment_2d.glsl");
    core->ShaderLibrary->Get("shader_2d_1024_768")->use();
    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(BAE::Defaults::BASE_SCREEN_WIDTH), static_cast<float>(BAE::Defaults::BASE_SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
    core->ShaderLibrary->Get("shader_2d_1024_768")->setMat4("projection", projectionMatrix);
    core->ShaderLibrary->Get("shader_2d_1024_768")->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White color

    // 256 x 768 2D shader
    core->ShaderLibrary->Add("shader_2d_256_768", "vertex_2d.glsl", "fragment_2d.glsl");
    core->ShaderLibrary->Get("shader_2d_256_768")->use();
    projectionMatrix = glm::ortho(0.0f, static_cast<float>(256), static_cast<float>(768), 0.0f, -1.0f, 1.0f);
    core->ShaderLibrary->Get("shader_2d_256_768")->setMat4("projection", projectionMatrix);
    core->ShaderLibrary->Get("shader_2d_256_768")->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White color

    // 3D shader
    core->ShaderLibrary->Add("shader_3d", "vertex_model_3d.glsl", "fragment_model_3d.glsl");

    // Textures
    core->TextureLibrary->Add("base_font", "assets/ExportedFont.bmp");
    core->TextureLibrary->Add("test_tileset", "assets/tilemap/tilesheet.png");

    /*-------------
    INITIALIZATIONS
    -------------*/
    Sint32 curScreenWidth = BAE::Defaults::BASE_SCREEN_WIDTH;
    Sint32 curScreenHeight = BAE::Defaults::BASE_SCREEN_HEIGHT;
    int offsetX = 0;
    int offsetY = 0;
    std::vector<int> map = {
        1, 1, 1, 1, 1, 1, 
        1, 0, 0, 0, 1, 0, 
        1, 0, 1, 0, 1, 0, 
        1, 0, 0, 0, 0, 0, 
        1, 1, 1, 1, 1, 0, 
        1, 0, 0, 0, 1, 2
    };
    int mapSize = 6;
    int playerRow = 5;
    int playerColumn = 5;
    auto camera3d = std::make_unique<BAE::Camera3D>(768.0f, 768.0f);

    auto meshGenerator = std::make_unique<BAE::GridMeshGenerator>(map, mapSize);
    auto renderer3dMap = std::make_unique<BAE::Renderer3D>(core->TextureLibrary->GetID("test_tileset"), meshGenerator->StaticVertices());
    auto renderer3dPlayer = std::make_unique<BAE::Renderer3D>(core->TextureLibrary->GetID("test_tileset"), meshGenerator->PlayerVertices());

    auto model3d = std::make_unique<BAE::Model>("assets/crates/Crate-04.FBX");

    /*-------------
    GAME LOOP
    -------------*/
    bool quit = false;

    while (!core->Quit())
    {
        core->Timer->BeginFrame();

        while (core->Event->Poll()) {
            // Shutdown
            if (core->Event->EQuit()) { core->BeginShutdown(); }

            //if (core->Event->Type() == SDL_KEYDOWN) {
            //    switch (core->Event->Sym()) {
            //    case SDLK_UP:
            //        if (attemptMove(playerRow - 1, playerColumn, map, 6)) {
            //            playerRow -= 1;
            //        }
            //        break;
            //    case SDLK_DOWN:
            //        if (attemptMove(playerRow + 1, playerColumn, map, 6)) {
            //            playerRow += 1;
            //        }
            //        break;
            //    case SDLK_LEFT:
            //        if (attemptMove(playerRow, playerColumn - 1, map, 6)) {
            //            playerColumn -= 1;
            //        }
            //        break;
            //    case SDLK_RIGHT:
            //        if (attemptMove(playerRow, playerColumn + 1, map, 6)) {
            //            playerColumn += 1;
            //        }
            //        break;
            //    }

            //    std::cout << "row: " << playerRow << "column: " << playerColumn << std::endl;
            //}

            camera3d->UpdateOrbit(core->Event->GetEvent(), core->Timer->DeltaTime());
        }
         
        while (core->Timer->ShouldUpdate()) {
            // update processes with delta time
        }

        core->BeginRender();

        /*-------------
        LEFT MESH VIEW
        -------------*/
        glViewport(0, 0, 768, 768);

        //renderer3dMap->render(
        //    *core->ShaderLibrary->Get("shader_3d"), 
        //    *camera3d, glm::vec3(0, 0, 0)
        //);

        //renderer3dPlayer->render(
        //    *core->ShaderLibrary->Get("shader_3d"),
        //    *camera3d, glm::vec3(0, 0, 0)
        //);

        core->ShaderLibrary->Get("shader_3d")->use();
        core->ShaderLibrary->Get("shader_3d")->setMat4("projection", camera3d->Projection());
        core->ShaderLibrary->Get("shader_3d")->setMat4("view", camera3d->View());

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
        core->ShaderLibrary->Get("shader_3d")->setMat4("model", model);

        model3d->Draw(*core->ShaderLibrary->Get("shader_3d"));

        /*-------------
        RIGHT TEXT VIEW
        -------------*/
        glViewport(768, 0, 256, 768);

        BAE::RendererText2D::Render(
            core->ShaderLibrary->GetID("shader_2d_256_768"),
            core->TextureLibrary->GetID("base_font"),
            std::to_string(core->Timer->DeltaTime()) + " ms",
            0,
            0,
            glm::vec3(1, 1, 1),
            1
        );

        BAE::RendererText2D::Render(
            core->ShaderLibrary->GetID("shader_2d_256_768"),
            core->TextureLibrary->GetID("base_font"),
            "Map Vertices\n" + std::to_string(meshGenerator->StaticVertices().size()),
            0,
            32,
            glm::vec3(1, 1, 1),
            1
        );

        BAE::RendererText2D::Render(
            core->ShaderLibrary->GetID("shader_2d_256_768"),
            core->TextureLibrary->GetID("base_font"),
            "Screen W: " + std::to_string(curScreenWidth),
            0,
            80,
            glm::vec3(1, 1, 1),
            1
        );

        BAE::RendererText2D::Render(
            core->ShaderLibrary->GetID("shader_2d_256_768"),
            core->TextureLibrary->GetID("base_font"),
            "Screen H: " + std::to_string(curScreenHeight),
            0,
            96,
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
