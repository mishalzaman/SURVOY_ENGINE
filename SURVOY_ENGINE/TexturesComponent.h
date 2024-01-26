#pragma once

#include <vector>
#include "Component.h"
#include "STexture.h"

using namespace BAE;

namespace ECS {
	class TexturesComponent : public Component
	{
	public:
		TexturesComponent(std::vector<STexture> textures);

		std::vector<STexture> Textures;
	};
}
