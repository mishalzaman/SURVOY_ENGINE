
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Core.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderQuad.h"
#include "RenderText.h"
#include "TextureLibrary.h"

int main(int argc, char* args[]) {
	auto core = std::make_unique<BAE::Core>();

	if (
		!core->CreateDevice(1024, 768, "test")
		) {
		return core->GetError();
	}
    core->StartDevice();

    core->TextureLibrary->Add("font", "assets/ExportedFont.bmp");
    core->TextureLibrary->Add("image_1", "assets/testbackground.bmp");
    core->TextureLibrary->Add("image_2", "assets/testbackground2.bmp");

	// loop
    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }

        core->BeginScene();

        core->Scene->Draw();
        BAE::RenderText::Render(
            core->ShaderLibrary->GetID("base"),
            core->TextureLibrary->GetID("font"),
            "123456ABCDEFG!@#$%",
            256,
            0,
            glm::vec3(1, 1, 1),
            1
        );
        BAE::RenderQuad::Render(
            core->ShaderLibrary->GetID("base"),
            core->TextureLibrary->GetID("image_1"),
            0,
            0,
            core->TextureLibrary->Get("image_1")->GetWidth() / 4,
            core->TextureLibrary->Get("image_1")->GetHeight() / 4,
            glm::vec3(1, 1, 1),
            1
        );
        BAE::RenderQuad::Render(
            core->ShaderLibrary->GetID("base"),
            core->TextureLibrary->GetID("image_2"),
            0,
            256,
            core->TextureLibrary->Get("image_2")->GetWidth() / 4,
            core->TextureLibrary->Get("image_2")->GetHeight() / 4,
            glm::vec3(1, 1, 1),
            1
        );

        core->EndScene();
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

	core->DestroyDevice();

	return BAE::Code::CORE_SUCCESS;
}
