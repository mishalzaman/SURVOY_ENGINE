#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>
#include "STexture.h"

namespace ENGINE {
	class FileLoader
	{
	public:
		static void Texture(GLuint& textureID, const std::string& path, int& width, int& height, int& channels);
		static void Texture(STexture& texture);
		static void TextureSkyBox(STexture& texture, const std::vector<std::string>& faces);
	};
}
