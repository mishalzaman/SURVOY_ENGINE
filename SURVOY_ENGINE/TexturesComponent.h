#pragma once

#include <vector>
#include "STexture.h"

using namespace BAE;

namespace ECS {
	struct TexturesComponent
	{
	public:
		TexturesComponent(std::vector<STexture> textures) :
			Textures(textures) {};

		std::vector<STexture> Textures;
	};
}
