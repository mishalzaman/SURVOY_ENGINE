#include "TextureLibrary.h"

BAE::TextureLibrary::TextureLibrary()
{
    _setDefaulFont();
}

void BAE::TextureLibrary::Add(const std::string& key, const std::string& bitmapFont)
{
	_textures[key] = std::make_unique<Texture>(bitmapFont);
}

BAE::Texture* BAE::TextureLibrary::Get(const std::string& key)
{
    auto it = _textures.find(key);
    if (it != _textures.end()) {
        return it->second.get(); // Return the raw pointer of the Shader
    }
    return nullptr;
}

GLuint BAE::TextureLibrary::GetID(const std::string& key)
{
    auto it = _textures.find(key);
    if (it != _textures.end()) {
        return it->second.get()->GetID(); // Return the raw pointer of the Shader
    }
    return 0;
}

void BAE::TextureLibrary::Remove(const std::string& key)
{
    _textures.erase(key);
}

void BAE::TextureLibrary::_setDefaulFont()
{
    Add("base_font", "assets/ExportedFont.bmp");
}
