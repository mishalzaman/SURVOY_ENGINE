#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include "STexture.h"

namespace BAE {
	class FileLoader
	{
	public:
		static void Texture(GLuint& textureID, const std::string& path, int& width, int& height, int& channels);
		static void Texture(STexture& texture);
	};
}
