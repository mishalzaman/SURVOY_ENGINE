#include "RenderPassDepthMapSystem.h"

ECS::RenderPassDepthMapSystem::RenderPassDepthMapSystem(EntityManager& entityManager):
	_entityManager(entityManager), _quadVertices{
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    }, _nearPlane(0.1f), _farPlane(64.f)
{
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getByTags("DepthQuadBuffers")[0]
    );

    glGenVertexArrays(1, &buffers->VAO);
    glGenBuffers(1, &buffers->VBO);
    glBindVertexArray(buffers->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
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

    lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, _nearPlane, _farPlane);

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
    ECS::LightSpaceMatrixComponent* lsm = _entityManager.getComponent<ECS::LightSpaceMatrixComponent>(
        _entityManager.getByTags("LightSpaceMatrix")[0]
    );
    ECS::CameraMatricesComponent* cameraMatrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(
        _entityManager.getByTags("CameraFreeLook")[0]
    );
    


    lightView = glm::lookAt(directionalLight->Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lsm->LightSpaceMatrix = lightProjection * lightView;

    // Render

    depthShader->Program.use();
    depthShader->Program.setMat4("lightSpaceMatrix", lsm->LightSpaceMatrix);

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

    _renderDepthDebugQuad();
}

void ECS::RenderPassDepthMapSystem::Unload()
{
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getByTags("DepthQuadBuffers")[0]
    );

    glDeleteVertexArrays(1, &buffers->VAO);
    glDeleteBuffers(1, &buffers->VBO);
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

void ECS::RenderPassDepthMapSystem::_renderDepthDebugQuad()
{
    ECS::ProgramComponent* debugDepthQuadShader = _entityManager.getComponent<ECS::ProgramComponent>(
        _entityManager.getByTags("DebugDepthQuadShader")[0]
    );
    ECS::TextureComponent* depthTexture = _entityManager.getComponent<ECS::TextureComponent>(
        _entityManager.getByTags("DepthTexture")[0]
    );
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getByTags("DepthQuadBuffers")[0]
    );

    glViewport(0, 0, ENGINE::Defaults::BASE_SCREEN_WIDTH, ENGINE::Defaults::BASE_SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render debug quad
    debugDepthQuadShader->Program.use();
    debugDepthQuadShader->Program.setFloat("near_plane", _nearPlane);
    debugDepthQuadShader->Program.setFloat("far_plane", _farPlane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture->Texture.id);

    glBindVertexArray(buffers->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
