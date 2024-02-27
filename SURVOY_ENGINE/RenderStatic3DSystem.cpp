#include "RenderStatic3DSystem.h"

ECS::RenderStatic3DSystem::RenderStatic3DSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager):
    _eventManager(eventManager),
    _entityManager(entityManager),
    _physics(physics)
{
    _eventManager.subscribe(this);
}

ECS::RenderStatic3DSystem::~RenderStatic3DSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::RenderStatic3DSystem::onNotify(const Event& event)
{
}

void ECS::RenderStatic3DSystem::Load() {

}

void ECS::RenderStatic3DSystem::Render() {
    std::vector<int> entities = _entityManager.getByTags("Mesh");

    int e = _entityManager.getByTag("ShadowMapColourShader")[0];
    ECS::ProgramComponent* shader = _entityManager.getComponent<ECS::ProgramComponent>(e);

    ECS::DirectionalLightComponent* directionalLight = _entityManager.getComponent<ECS::DirectionalLightComponent>(
        _entityManager.getByTags("DirectionalLight")[0]
    );
    ECS::TextureComponent* depthTexture = _entityManager.getComponent<ECS::TextureComponent>(
        _entityManager.getByTags("DepthTexture")[0]
    );
    ECS::LightSpaceMatrixComponent* lsm = _entityManager.getComponent<ECS::LightSpaceMatrixComponent>(
        _entityManager.getByTags("LightSpaceMatrix")[0]
    );

    if (shader) {
        shader->Program.use();
        shader->Program.setVec3("lightPos", directionalLight->Position);
        shader->Program.setVec3("viewPos", _cameraPosition);
        shader->Program.setVec3("lightColor", glm::vec3(1, 1, 1));
        shader->Program.setMat4("projection", _projection);
        shader->Program.setMat4("view", _view);
        shader->Program.setInt("diffuseTexture", 0);
        shader->Program.setInt("shadowMap", 1);
        shader->Program.setMat4("lightSpaceMatrix", lsm->LightSpaceMatrix);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthTexture->Texture.id);
    }
    else {
        std::cout << "could not load shader" << std::endl;
    }

    for (int entityId : entities) {
        // Retrieve the components required for rendering
        ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        ECS::TexturesComponent* textures = _entityManager.getComponent<ECS::TexturesComponent>(entityId);



        if (transform && mesh && buffers && textures && shader) {
            shader->Program.setMat4("model", transform->Transformation);
            _render(*transform, *mesh, *buffers, *textures);
        }
    }
}

void ECS::RenderStatic3DSystem::Unload() {

}

void ECS::RenderStatic3DSystem::_render(
    const TransformComponent& transform,
    const MeshComponent& mesh,
    const BuffersComponent& buffers,
    const TexturesComponent& textures
)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures.Textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        glBindTexture(GL_TEXTURE_2D, textures.Textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(buffers.VAO);
    glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
