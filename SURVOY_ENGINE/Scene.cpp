#include "Scene.h"

using namespace BAE;

BAE::Scene::Scene(int screenW, int screenH):
    _defaultShader(nullptr)
{
    _screenW = screenW;
    _screenH = screenH;
    _setDefaultShader();
}

BAE::Scene::~Scene()
{
    delete _defaultShader;
}

void BAE::Scene::Draw()
{
    //for (auto& pair : _resources) {
    //    // The 'pair' is a std::pair<const std::string, std::unique_ptr<Resource>>
    //    // pair.first is the key (string)
    //    // pair.second is the value (unique_ptr to Resource)

    //    // Check if the resource is not null before calling Draw
    //    if (pair.second) {
    //        BAE::RenderQuad::Render(
    //            pair.second->GetShaderID(),
    //            pair.second->GetTextureID(),
    //            pair.second->GetPosition().x,
    //            pair.second->GetPosition().y,
    //            pair.second->GetWidth(),
    //            pair.second->GetHeight(),
    //            glm::vec3(1, 1, 1),
    //            pair.second->GetScale()
    //        );
    //    }
    //}
}

void BAE::Scene::_setDefaultShader()
{
    _defaultShader = new BAE::Shader("base_vertex.glsl", "base_fragment.glsl");
    _defaultShader->use();
    // Set shader uniforms as needed, e.g., projection matrix, textColor

    // Create an orthographic projection matrix
    glm::mat4 projectionMatrix = glm::ortho(0.0f, _screenW, _screenH, 0.0f, -1.0f, 1.0f);

    _defaultShader->setMat4("projection", projectionMatrix);
    _defaultShader->setVec3("textColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White color
}
