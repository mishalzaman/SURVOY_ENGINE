#include "ShaderLibrary.h"

void BAE::ShaderLibrary::Add(const std::string& key, const char* vertexPath, const char* fragmentPath)
{
	_shaders[key] = std::make_unique<Shader>(vertexPath, fragmentPath);
}

BAE::Shader * BAE::ShaderLibrary::GetDefault()
{
    auto it = _shaders.find("base");
    if (it != _shaders.end()) {
        return it->second.get(); // Return the raw pointer of the Shader
    }
    return nullptr;
}

BAE::Shader* BAE::ShaderLibrary::Get(const std::string& key)
{
    auto it = _shaders.find(key);
    if (it != _shaders.end()) {
        return it->second.get(); // Return the raw pointer of the Shader
    }
    return nullptr;
}

GLuint BAE::ShaderLibrary::GetID(const std::string& key)
{
    auto it = _shaders.find(key);
    if (it != _shaders.end()) {
        return it->second.get()->ID; // Return the raw pointer of the Shader
    }
    return 0;
}

void BAE::ShaderLibrary::Remove(const std::string& key)
{
	_shaders.erase(key);
}
