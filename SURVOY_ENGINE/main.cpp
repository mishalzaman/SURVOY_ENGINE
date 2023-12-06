
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Core.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderQuad.h"
#include "RenderText.h"

int main(int argc, char* args[]) {
	auto core = std::make_unique<ENGINE::Core>();

	if (
		!core->CreateDevice(1024, 768, "test")
		) {
		return core->GetError();
	}
    core->StartDevice();

    //core->Scene->AddResource("image1", "assets/font.png");
    //core->Scene->GetResource("image1")->SetPosition(glm::vec2(100, 100));
    //core->Scene->GetResource("image1")->SetScale(2);

    // TESTING FONTS
    ENGINE::Shader* shader = new ENGINE::Shader("base_vertex.glsl", "base_fragment.glsl");
    shader->use();
    // Set shader uniforms as needed, e.g., projection matrix, textColor

    // Create an orthographic projection matrix
    glm::mat4 projectionMatrix = glm::ortho(0.0f, 1024.f, 768.f, 0.0f, -1.0f, 1.0f);

    shader->setMat4("projection", projectionMatrix);
    shader->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White color


    GLuint fTexture;
    int width;
    int height;
    int channel;
    ENGINE::Texture::Load(fTexture, "assets/ExportedFont.bmp", width, height, channel);

    // END TESTING FONTS

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
        // Optionally, you can convert 'err' to a string for more descriptive output
    }

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
            ENGINE::RenderText::Render(shader->ID, fTexture, "123456ABCDEFG!@#$%", 0, 0, width, height, glm::vec3(1, 1, 1), 1);
            //ENGINE::RenderQuad::Render(shader->ID, fTexture, 0, 0, width, height, glm::vec3(1, 1, 1), 1);

            core->EndScene();
        }
    }

	core->DestroyDevice();

	return ENGINE::Code::CORE_SUCCESS;
}
