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
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getIdByTag("DepthQuadBuffers")
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
        _entityManager.getIdByTag("DepthFBO")
    );
    ECS::TextureComponent* texture = _entityManager.getComponent<ECS::TextureComponent>(
        _entityManager.getIdByTag("DepthTexture")
    );
    ECS::ShadowResolutionComponent* shadowResolution = _entityManager.getComponent<ECS::ShadowResolutionComponent>(
        _entityManager.getIdByTag("DepthTexture")
    );

    if (buffers && texture && shadowResolution) {
        glGenFramebuffers(1, &buffers->DepthFBO);
        std::cout << "RenderPassDepthMapSystem::Load DepthFBO: " << buffers->DepthFBO << std::endl;

        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            shadowResolution->Width, shadowResolution->Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
    ECS::RenderPassComponent* renderPipeline = _entityManager.getComponent<ECS::RenderPassComponent>(
        _entityManager.getIdByTag("RenderPipeline")
    );

    renderPipeline->State = ECS::RenderPassComponent::SHADOW_MAP;
}

void ECS::RenderPassDepthMapSystem::Unload()
{
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getIdByTag("DepthQuadBuffers")
    );

    glDeleteVertexArrays(1, &buffers->VAO);
    glDeleteBuffers(1, &buffers->VBO);
}
