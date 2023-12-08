#include "ShaderLibrary.h"

BAE::ShaderLibrary::ShaderLibrary(int screenW, int screenH):
    _screenW(screenW),
    _screenH(screenH)
{
    _setDefault();
}

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

void BAE::ShaderLibrary::_setDefault()
{
    Add("base_shader", "base_vertex.glsl", "base_fragment.glsl");
    Get("base_shader")->use();
    glm::mat4 projectionMatrix = glm::ortho(0.0f, 1024.f, 768.f, 0.0f, -1.0f, 1.0f);
    Get("base_shader")->setMat4("projection", projectionMatrix);
    Get("base_shader")->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White color
}
