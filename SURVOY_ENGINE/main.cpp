
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core.h"

int main(int argc, char* args[]) {
	auto core = std::make_unique<ENGINE::Core>();

	if (
		!core->CreateDevice(1024, 768, "test")
		) {
		return core->GetError();
	}
    core->StartDevice();

    core->Scene->AddResource("image1", "assets/font.png");
    //core->Scene->AddResource("image2", "assets/testbackground2.bmp");

    core->Scene->GetResource("image1")->SetPosition(glm::vec2(100, 100));
    core->Scene->GetResource("image1")->SetScale(2);
    //core->Scene->GetResource("image2")->SetPosition(glm::vec2(356, 100));

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

            core->Scene->Draw();

            core->EndScene();
        }
    }

	core->DestroyDevice();

	return ENGINE::Code::CORE_SUCCESS;
}
