
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
	auto core = std::make_unique<ENGINE::Core>();

	if (
		!core->CreateDevice(1024, 768, "test")
		) {
		return core->GetError();
	}
    core->StartDevice();

    core->TextureLibrary->Add("font", "assets/ExportedFont.bmp");

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

            core->BeginScene();

            //core->Scene->Draw();
            ENGINE::RenderText::Render(
                core->ShaderLibrary->GetID("base"),
                core->TextureLibrary->GetID("font"),
                "123456ABCDEFG!@#$%",
                0,
                0,
                glm::vec3(1, 1, 1),
                1
            );
            ENGINE::RenderQuad::Render(
                core->ShaderLibrary->GetID("base"),
                core->TextureLibrary->GetID("font"),
                0,
                100,
                core->TextureLibrary->Get("font")->GetWidth(),
                core->TextureLibrary->Get("font")->GetHeight(),
                glm::vec3(1, 1, 1),
                1
            );

            core->EndScene();
        }
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

	core->DestroyDevice();

	return ENGINE::Code::CORE_SUCCESS;
}
