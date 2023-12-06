#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

namespace ENGINE {
	class Texture
	{
	public:
		static bool Load(GLuint& fontTexture, const std::string& bitmapFont, int& width, int& height, int& channels);
	};
}

