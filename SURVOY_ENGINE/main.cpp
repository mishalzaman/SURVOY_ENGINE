
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Core.h"
#include "RenderQuad.h"
#include "RenderText.h"
#include "Event.h"

int main(int argc, char* args[]) {
	auto core = std::make_unique<BAE::Core>();

	if (
		!core->CreateDevice(1024, 768, "BAE 0.1.0")
		) {
		return core->GetError();
	}

    core->TextureLibrary->Add("image_1", "assets/testbackground.bmp");
    core->TextureLibrary->Add("image_2", "assets/testbackground2.bmp");
    core->TextureLibrary->Add("test_template", "assets/test_1024_768.bmp");

	// loop
    bool quit = false;

    std::string uInput;

    while (!core->Quit())
    {
        core->Timer->BeginFrame();

        while (core->Event->Poll()) {
            switch (core->Event->Type())
            {
            case SDL_QUIT:
                core->PerformShutdown();
                break;
            case SDL_KEYDOWN:
                if (core->Event->isBackSpace()) {
                    if (!uInput.empty()) {
                        uInput.pop_back();
                    }
                }
                break;
            case SDL_TEXTINPUT:
                uInput += core->Event->Text();
                break;
            }
        }

        while (core->Timer->ShouldUpdate()) {
            // update processes with delta time
        }

        core->BeginRender();

        BAE::RenderQuad::Render(
            core->ShaderLibrary->GetID("base_shader"),
            core->TextureLibrary->GetID("test_template"),
            0,
            0,
            1024,
            768,
            1
        );

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
