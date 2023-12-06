#include "TextureLibrary.h"

void ENGINE::TextureLibrary::Add(const std::string& key, const std::string& bitmapFont)
{
	_textures[key] = std::make_unique<Texture>(bitmapFont);
}

ENGINE::Texture* ENGINE::TextureLibrary::Get(const std::string& key)
{
    auto it = _textures.find(key);
    if (it != _textures.end()) {
        return it->second.get(); // Return the raw pointer of the Shader
    }
    return nullptr;
}

GLuint ENGINE::TextureLibrary::GetID(const std::string& key)
{
    auto it = _textures.find(key);
    if (it != _textures.end()) {
        return it->second.get()->GetID(); // Return the raw pointer of the Shader
    }
    return 0;
}

void ENGINE::TextureLibrary::Remove(const std::string& key)
{
    _textures.erase(key);
}
