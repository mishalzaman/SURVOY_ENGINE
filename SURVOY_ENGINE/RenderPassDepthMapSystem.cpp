#include "RenderPassDepthMapSystem.h"

ECS::RenderPassDepthMapSystem::RenderPassDepthMapSystem(EntityManager& entityManager):
	_entityManager(entityManager), _quadVertices{
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    }
{
    glGenVertexArrays(1, &_quadVAO);
    glGenBuffers(1, &_quadVBO);
    glBindVertexArray(_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
    glBufferData(GL_ARRAY_BUFFER, _quadVertices.size() * sizeof(float), _quadVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void ECS::RenderPassDepthMapSystem::Load()
{
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getByTags("DepthFBO")[0]
    );
    ECS::TextureComponent* texture = _entityManager.getComponent<ECS::TextureComponent>(
        _entityManager.getByTags("DepthTexture")[0]
    );

    if (buffers && texture) {
        glGenFramebuffers(1, &buffers->DepthFBO);
        std::cout << "RenderPassDepthMapSystem::Load DepthFBO: " << buffers->DepthFBO << std::endl;

        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        texture->Texture.id = depthMap;
        std::cout << "RenderPassDepthMapSystem::Load texture id: " << texture->Texture.id << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, buffers->DepthFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->Texture.id, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void ECS::RenderPassDepthMapSystem::Render()
{
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;

    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    ECS::DirectionalLightComponent* directionalLight = _entityManager.getComponent<ECS::DirectionalLightComponent>(
        _entityManager.getByTags("DirectionalLight")[0]
    );

    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getByTags("DepthFBO")[0]
    );
    ECS::TextureComponent* depthTexture = _entityManager.getComponent<ECS::TextureComponent>(
        _entityManager.getByTags("DepthTexture")[0]
    );
    ECS::ProgramComponent* depthShader = _entityManager.getComponent<ECS::ProgramComponent>(
        _entityManager.getByTags("DepthShader")[0]
    );
    ECS::ProgramComponent* debugDepthQuadShader = _entityManager.getComponent<ECS::ProgramComponent>(
        _entityManager.getByTags("DebugDepthQuadShader")[0]
    );

    lightView = glm::lookAt(directionalLight->Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    // Render

    depthShader->Program.use();
    depthShader->Program.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, buffers->DepthFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);

    // render meshes begin

    std::vector<int> entities = _entityManager.getByTags("Mesh");
    for (int entityId : entities) {
        // Retrieve the components required for rendering
        ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        ECS::TexturesComponent* textures = _entityManager.getComponent<ECS::TexturesComponent>(entityId);

        if (transform && mesh && buffers && textures) {
            depthShader->Program.setMat4("model", transform->Transformation);
            _renderMeshes(*transform, *mesh, *buffers, *textures);
        }
    }

    // render meshes end

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, ENGINE::Defaults::BASE_SCREEN_WIDTH, ENGINE::Defaults::BASE_SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render debug quad
    debugDepthQuadShader->Program.use();
    debugDepthQuadShader->Program.setFloat("near_plane", near_plane);
    debugDepthQuadShader->Program.setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture->Texture.id);

    glBindVertexArray(_quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void ECS::RenderPassDepthMapSystem::Unload()
{
    glDeleteVertexArrays(1, &_quadVAO);
    glDeleteBuffers(1, &_quadVBO);
}

void ECS::RenderPassDepthMapSystem::_renderMeshes(const TransformComponent& transform, const MeshComponent& mesh, const BuffersComponent& buffers, const TexturesComponent& textures)
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
