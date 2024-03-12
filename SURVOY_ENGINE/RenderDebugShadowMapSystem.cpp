#include "RenderDebugShadowMapSystem.h"

ECS::RenderDebugShadowMapSystem::RenderDebugShadowMapSystem(EntityManager& entityManager):
    _entityManager(entityManager)
{
}

void ECS::RenderDebugShadowMapSystem::Render()
{
    glViewport(856, 0, 168, 168);

    ECS::ProgramComponent* debugDepthQuadShader = _entityManager.getComponent<ECS::ProgramComponent>(
        _entityManager.getIdByTag("DebugDepthQuadShader")
    );
    ECS::TextureComponent* depthTexture = _entityManager.getComponent<ECS::TextureComponent>(
        _entityManager.getIdByTag("DepthTexture")
    );
    ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(
        _entityManager.getIdByTag("DepthQuadBuffers")
    );
    ECS::NearFarPlanesComponent* planes = _entityManager.getComponent<ECS::NearFarPlanesComponent>(
        _entityManager.getIdByTag("DepthTexture")
    );

    // render debug quad
    debugDepthQuadShader->Program.use();
    debugDepthQuadShader->Program.setFloat("near_plane", planes->Near);
    debugDepthQuadShader->Program.setFloat("far_plane", planes->Far);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture->Texture.id);

    glBindVertexArray(buffers->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glViewport(0, 0, ENGINE::Defaults::BASE_SCREEN_WIDTH, ENGINE::Defaults::BASE_SCREEN_HEIGHT);
}
