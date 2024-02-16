#pragma once

#include <vector>
#include "STexture.h"

using namespace ENGINE;

namespace ECS {
	struct TexturesComponent
	{
	public:
		TexturesComponent(std::vector<STexture> textures) :
			Textures(textures) {};

		std::vector<STexture> Textures;
	};
}
