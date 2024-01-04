
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Core.h"
#include "RendererText2D.h"
#include "RendererQuad2D.h"
#include "Defaults.h"
#include "Camera3D.h"
#include "Camera2D.h"
#include "Model.h"
#include "Shader.h"
#include "STexture.h"
#include "FileLoader.h"

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

    /*=============
    INITIALIZATIONS
    =============*/

    /*------------
    3D
    -------------*/
    auto camera3d = std::make_unique<BAE::Camera3D>(768.0f, 768.0f);

    // MODEL
    auto model3d = std::make_unique<BAE::Model>("assets/muffin/muffin.obj");
    auto shader3D = std::make_unique<BAE::Shader>("vertex_model_3d.glsl", "fragment_model_3d.glsl");

    /*------------
    2D
    ------------*/
    auto camera2d = std::make_unique<BAE::Camera2D>(256.0f, 768.0f);

    // FONT
    auto fontTexture = std::make_unique<BAE::STexture>();
    GLuint fTextureId;
    int fWidth;
    int fHeight;
    int fChannels;
    std::string fPath = "assets/ExportedFont.bmp";
    
    BAE::FileLoader::Texture(fTextureId, fPath, fWidth, fHeight, fChannels);

    fontTexture->id = fTextureId;
    fontTexture->width = fWidth;
    fontTexture->height = fHeight;
    fontTexture->channel = fChannels;
    fontTexture->path = fPath;
    
    auto shader2D = std::make_unique<BAE::Shader>("vertex_2d.glsl", "fragment_2d.glsl");

    // QUAD
    auto imageTexture = std::make_unique<BAE::STexture>();
    imageTexture->path = "assets/tilemap/tilesheet.png";
    BAE::FileLoader::Texture(*imageTexture);

    auto shader2DQuad = std::make_unique<BAE::Shader>("base_vertex_2d.glsl", "base_fragment_2d.glsl");

    auto renderQuad = std::make_unique<BAE::RendererQuad2D>(*imageTexture, shader2DQuad->ID, 0, 128, 1);

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
        }
         
        while (core->Timer->ShouldUpdate()) {
            camera3d->UpdateOrbit(core->Event->GetEvent(), core->Timer->DeltaTime());
        }

        core->BeginRender();

        /*=============
        LEFT MESH VIEW
        =============*/
        glViewport(0, 0, 768, 768);

        // 3D model
        shader3D->use();
        shader3D->setMat4("projection", camera3d->Projection());
        shader3D->setMat4("view", camera3d->View());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader3D->setMat4("model", model);

        model3d->Draw(*shader3D);

        /*=============
        RIGHT TEXT VIEW
        =============*/
        glViewport(768, 0, 256, 768);

        shader2D->use();
        glm::mat4 projectionMatrix = camera2d->Projection();
        shader2D->setMat4("projection", projectionMatrix);
        shader2D->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f));

        BAE::RendererText2D::Render(
            shader2D->ID,
            fontTexture->id,
            std::to_string(core->Timer->DeltaTime()) + " ms",
            0,
            0,
            glm::vec3(1, 1, 1),
            1
        );

        BAE::RendererText2D::Render(
            shader2D->ID,
            fontTexture->id,
            "VER: " + std::to_string(model3d->NumVertices()),
            0,
            16,
            glm::vec3(1, 1, 1),
            1
        );

        BAE::RendererText2D::Render(
            shader2D->ID,
            fontTexture->id,
            "TRI: " + std::to_string(model3d->NumVertices()/3),
            0,
            32,
            glm::vec3(1, 1, 1),
            1
        );

        shader2DQuad->use();
        projectionMatrix = camera2d->Projection();
        shader2DQuad->setMat4("projection", projectionMatrix);

        renderQuad->Render();

        core->EndRender();
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

	core->DestroyDevice();

	return BAE::Code::CORE_SUCCESS;
}
