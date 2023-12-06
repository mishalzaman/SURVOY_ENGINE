#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

namespace ENGINE {
	class Texture
	{
	public:
		Texture(const std::string& bitmapFont);
		~Texture();
		static bool Load(GLuint& fontTexture, const std::string& bitmapFont, int& width, int& height, int& channels);

		GLuint GetID() { return _textureId; }

	private:
		GLuint _textureId;
		int _width;
		int _height;
		int _channels;

		void _import(GLuint& fontTexture, const std::string& bitmapFont, int& width, int& height, int& channels);
	};
}

