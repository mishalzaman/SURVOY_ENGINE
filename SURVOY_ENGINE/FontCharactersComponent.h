#pragma once

#include <gl/glew.h>
#include <map>
#include <glm/glm.hpp>

struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

namespace ECS {
	struct FontCharactersComponent
	{
	public:
		FontCharactersComponent() {}

		std::map<GLchar, Character> Characters;
	};
}
