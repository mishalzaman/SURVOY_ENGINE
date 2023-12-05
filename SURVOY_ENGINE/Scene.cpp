#include "Scene.h"

using namespace ENGINE;

ENGINE::Scene::Scene(int screenW, int screenH):
    _defaultShader(nullptr)
{
    _screenW = screenW;
    _screenH = screenH;
    _setDefaultShader();
}

ENGINE::Scene::~Scene()
{
    delete _defaultShader;
}

void ENGINE::Scene::AddResource(const std::string& key, const std::string& path)
{
	auto resource = std::make_unique<Resource>();
	resource->Create(path);
    resource->SetShader(_defaultShader->ID);

	_resources[key] = std::move(resource);
}

void ENGINE::Scene::DestroyResource(const std::string& key)
{
    _resources.erase(key);
}

Resource* ENGINE::Scene::GetResource(const std::string& key)
{
    auto it = _resources.find(key);
    if (it != _resources.end()) {
        return it->second.get();
    }
    return nullptr; // or throw an exception
}

void ENGINE::Scene::Draw()
{
    for (auto& pair : _resources) {
        // The 'pair' is a std::pair<const std::string, std::unique_ptr<Resource>>
        // pair.first is the key (string)
        // pair.second is the value (unique_ptr to Resource)

        // Check if the resource is not null before calling Draw
        if (pair.second) {
            pair.second->Draw();
        }
    }
}

void ENGINE::Scene::_setDefaultShader()
{
    _defaultShader = new ENGINE::Shader("base_vertex.glsl", "base_fragment.glsl");
    _defaultShader->use();
    // Set shader uniforms as needed, e.g., projection matrix, textColor

    // Create an orthographic projection matrix
    glm::mat4 projectionMatrix = glm::ortho(0.0f, _screenW, _screenH, 0.0f, -1.0f, 1.0f);

    _defaultShader->setMat4("projection", projectionMatrix);
    _defaultShader->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White color
}
