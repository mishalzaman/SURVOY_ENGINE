
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
#include <cstdlib> // Required for rand() and srand()
#include <ctime>   // Required for time()

int main(int argc, char* args[]) {
	auto core = std::make_unique<BAE::Core>();

	if (
		!core->CreateDevice(1024, 768, "BAE 0.1.0")
		) {
		return core->GetError();
	}

    core->TextureLibrary->Add("image_1", "assets/testbackground.bmp");
    core->TextureLibrary->Add("image_2", "assets/testbackground2.bmp");

	// loop
    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        core->Timer->BeginFrame();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }

        while (core->Timer->ShouldUpdate()) {
            // update processes with delta time
        }

        core->BeginRender();

        BAE::RenderText::Render(
            core->ShaderLibrary->GetID("base_shader"),
            core->TextureLibrary->GetID("base_font"),
            std::to_string(core->Timer->DeltaTime()),
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
