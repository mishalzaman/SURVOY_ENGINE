#include "RenderPassColourMapSystem.h"

ECS::RenderPassColourMapSystem::RenderPassColourMapSystem(EntityManager& entityManager, EventManager& eventManager) :
    _FBO(0),
    _RBO(0),
    _textureColorbuffer(0),
    _eventManager(eventManager),
    _entityManager(entityManager)
{
    _eventManager.subscribe(this);
}

ECS::RenderPassColourMapSystem::~RenderPassColourMapSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::RenderPassColourMapSystem::onNotify(const Event& event)
{
}

void ECS::RenderPassColourMapSystem::Load()
{
    ECS::BuffersComponent* buffer = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getIdByTag("ColourFBO")
    );
    ECS::TextureComponent* texture = _entityManager.getComponent<ECS::TextureComponent>(
        _entityManager.getIdByTag("ColourTexture")
    );

    /*==========================
    1. Create Framebuffer Object
    ==========================*/
    glGenFramebuffers(1, &buffer->VBO);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer->VBO);

    /*==========================
    2. Create colour texture
    ==========================*/
    glGenTextures(1, &texture->Texture.id);
    glBindTexture(GL_TEXTURE_2D, texture->Texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ENGINE::Defaults::BASE_SCREEN_WIDTH, ENGINE::Defaults::BASE_SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->Texture.id, 0);

    /*==============================================================
    4. Create a renderbuffer object for depth and stencil attachment
    ===============================================================*/
    glGenRenderbuffers(1, &_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, _RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ENGINE::Defaults::BASE_SCREEN_WIDTH, ENGINE::Defaults::BASE_SCREEN_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RBO); // now actually attach it

    /*==============================================================
    5. Now that we actually created the framebuffer and added all
       attachments we want to check if it is actually complete now
    ===============================================================*/
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ECS::RenderPassColourMapSystem::Render()
{
    ECS::RenderPassComponent* renderPipeline = _entityManager.getComponent<ECS::RenderPassComponent>(
        _entityManager.getByTags("RenderPipeline")[0]
    );

    renderPipeline->State = ECS::RenderPassComponent::COLOUR_MAP;
}

void ECS::RenderPassColourMapSystem::Unload()
{
    glDeleteRenderbuffers(1, &_RBO);
    glDeleteFramebuffers(1, &_FBO);
}