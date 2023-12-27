
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Core.h"
#include "RenderQuad.h"
#include "RenderText.h"
#include "RenderTileset.h"
#include "Render3D.h"
#include "MapLoader.h"
#include "Defaults.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "MeshGenerator.h"
int main(int argc, char* args[]) {
	auto core = std::make_unique<BAE::Core>();

	if (
		!core->CreateDevice("BAE 0.1.0")
		) {
		return core->GetError();
	}

    core->TextureLibrary->Add("image_1", "assets/testbackground.bmp");
    core->TextureLibrary->Add("image_2", "assets/testbackground2.bmp");
    core->TextureLibrary->Add("test_template", "assets/test_1024_768.bmp");
    core->TextureLibrary->Add("tileset", "assets/map/tileset.png");
    core->TextureLibrary->Add("tileset_test", "assets/map_generator/tileset.png");
    core->TextureLibrary->Add("wall", "assets/wall.jpg");
    core->TextureLibrary->Add("test_tileset", "assets/tilemap/tilesheet.png");

    core->ShaderLibrary->Add("shader_3d", "vertex_3d.glsl", "fragment_3d.glsl");

	// loop
    bool quit = false;

    std::string uInput;

    // map
    //auto mapLoader = std::make_unique<MapLoader>();
    //mapLoader->Load("assets/map/map1.tmj");
    //const std::vector<int>& graphicData = mapLoader->GetGraphic();

    // Camera / Renderer
    auto camera = std::make_unique<BAE::Camera3D>();

    std::vector<int> map = {
        0, 0, 0, 1,
        0, 1, 0, 0,
        0, 0, 1, 1,
        1, 0, 0, 1
    };
    auto meshGenerator = std::make_unique<BAE::MeshGenerator>(map, 4);
    auto renderer3d = std::make_unique<BAE::Renderer3D>(core->TextureLibrary->GetID("test_tileset"), meshGenerator->GetVertices());


    while (!core->Quit())
    {
        core->Timer->BeginFrame();

        while (core->Event->Poll()) {
            // Shutdown
            if (core->Event->EQuit()) { core->BeginShutdown(); }

            // Tey input
            if (core->Event->EKeyDown()) {
                if (core->Event->isBackSpace()) {
                    if (!uInput.empty()) {
                        uInput.pop_back();
                    }
                }
            }

            // Text input
            if (core->Event->ETextInput()) {
                uInput += core->Event->Text();
            }

            core->EventWindowResize();

            camera->UpdateOrbit(core->Event->GetEvent(), core->Timer->DeltaTime());
        }
         
        while (core->Timer->ShouldUpdate()) {
            // update processes with delta time
        }

        core->BeginRender();

        //BAE::RenderTileset::Render(
        //    core->ShaderLibrary->GetID("base_shader"),
        //    core->TextureLibrary->GetID("tileset"),
        //    graphicData,
        //    0,
        //    0,
        //    mapLoader->GetTileWidth(),
        //    mapLoader->GetTileHeight(),
        //    mapLoader->GetMapWidth(),
        //    mapLoader->GetMapHeight(),
        //    16,
        //    6,
        //    1
        //);

        renderer3d->render(*core->ShaderLibrary->Get("shader_3d"), *camera, glm::vec3(0,0,0));

        BAE::RenderText::Render(
            core->ShaderLibrary->GetID("base_shader"),
            core->TextureLibrary->GetID("base_font"),
            std::to_string(core->Timer->DeltaTime()),
            0,
            752,
            glm::vec3(1, 1, 1),
            1
        );

        //BAE::RenderQuad::Render(
        //    core->ShaderLibrary->GetID("base_shader"),
        //    core->TextureLibrary->GetID("wall"),
        //    0,
        //    0,
        //    512,
        //    512,
        //    1
        //);

        core->EndRender();
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

	core->DestroyDevice();

	return BAE::Code::CORE_SUCCESS;
}
