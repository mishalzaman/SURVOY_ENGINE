#include "FBORendererSystem.h"

ECS::FBORendererSystem::FBORendererSystem(EventManager& eventManager) : _quadVertices{
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    },
    _eventManager(eventManager),
    _colourBuffer(0)
{
    _shader = std::make_unique<Shader>("framebuffer_vertex.glsl", "framebuffer_fragment.glsl");
    _shader->use();
    _shader->setInt("screenTexture", 0);

    _eventManager.subscribe(this);
}

ECS::FBORendererSystem::~FBORendererSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::FBORendererSystem::onNotify(const Event& event)
{
    const auto* colourBufferEvent = dynamic_cast<const FrameBufferColourBufferEvent*>(&event);
    if (colourBufferEvent) {
        _colourBuffer = colourBufferEvent->getColourBuffer();
    }
}

void ECS::FBORendererSystem::Load()
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _quadVertices.size() * sizeof(float), _quadVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void ECS::FBORendererSystem::UpdatePrePhysics()
{
}

void ECS::FBORendererSystem::Update(float deltaTime)
{
}

void ECS::FBORendererSystem::UpdatePostPhysics()
{
}

void ECS::FBORendererSystem::Render()
{
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    _shader->use();
    glBindVertexArray(_VAO);
    glBindTexture(GL_TEXTURE_2D, _colourBuffer);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ECS::FBORendererSystem::Unload()
{
}
