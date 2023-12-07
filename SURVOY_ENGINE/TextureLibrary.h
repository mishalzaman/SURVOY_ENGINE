#pragma once

#include <map>
#include <string>
#include <memory>
#include "Texture.h"

namespace BAE {
	class TextureLibrary
	{
	public:
		void Add(const std::string& key, const std::string& bitmapFont);
		Texture* Get(const std::string& key);
		GLuint GetID(const std::string& key);
		void Remove(const std::string& key);
	private:
		std::map<std::string, std::unique_ptr<Texture>> _textures;
	};
}
