#include "ShaderLibrary.h"

void ENGINE::ShaderLibrary::Add(const std::string& key, const char* vertexPath, const char* fragmentPath)
{
	_shaders[key] = std::make_unique<Shader>(vertexPath, fragmentPath);
}

ENGINE::Shader * ENGINE::ShaderLibrary::GetDefault()
{
    auto it = _shaders.find("base");
    if (it != _shaders.end()) {
        return it->second.get(); // Return the raw pointer of the Shader
    }
    return nullptr;
}

ENGINE::Shader* ENGINE::ShaderLibrary::Get(const std::string& key)
{
    auto it = _shaders.find(key);
    if (it != _shaders.end()) {
        return it->second.get(); // Return the raw pointer of the Shader
    }
    return nullptr;
}

GLuint ENGINE::ShaderLibrary::GetID(const std::string& key)
{
    auto it = _shaders.find(key);
    if (it != _shaders.end()) {
        return it->second.get()->ID; // Return the raw pointer of the Shader
    }
    return 0;
}

void ENGINE::ShaderLibrary::Remove(const std::string& key)
{
	_shaders.erase(key);
}
