#pragma once

#include "STexture.h" // Assuming this is the file name where STexture is defined

namespace ECS {
	struct TextureComponent {
	public:
		TextureComponent() : Texture{0, "defaultType", "defaultPath", 0, 0, 0} {}

		ENGINE::STexture Texture;
	};
}
