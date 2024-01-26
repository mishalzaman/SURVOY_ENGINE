#include "MeshRenderSystem.h"

void ECS::MeshRenderSystem::_render(const TransformComponent& transform, const MeshComponent& mesh, const BuffersComponent& buffers, const TexturesComponent& textures)
{
    std::cout << buffers.VBO << std::endl;
}

void ECS::MeshRenderSystem::_initBuffers(const MeshComponent& mesh, BuffersComponent& buffers)
{
    glGenVertexArrays(1, &buffers.VAO);
    glGenBuffers(1, &buffers.VBO);
    glGenBuffers(1, &buffers.EBO);

    glBindVertexArray(buffers.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);

    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(SVertex), &mesh.Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(unsigned int), &mesh.Indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, TexCoords));

    glBindVertexArray(0);
}

void ECS::MeshRenderSystem::Update(float deltaTime, std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities)
{
    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        // Retrieve the components required for rendering
        TransformComponent* transform = _entityManager.getComponent<TransformComponent>(entityId);
        MeshComponent* mesh = _entityManager.getComponent<MeshComponent>(entityId);
        BuffersComponent* buffers = _entityManager.getComponent<BuffersComponent>(entityId);
        TexturesComponent* textures = _entityManager.getComponent<TexturesComponent>(entityId);

        if (transform && mesh && buffers && textures) {
            _render(*transform, *mesh, *buffers, *textures);
        }
    }
}

void ECS::MeshRenderSystem::Load(std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& entities)
{
    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        std::cout << entityId << std::endl;

        // Retrieve the components required for rendering
        BuffersComponent* buffers = _entityManager.getComponent<BuffersComponent>(entityId);
        MeshComponent* mesh = _entityManager.getComponent<MeshComponent>(entityId);

        if (mesh && buffers) {
            _initBuffers(*mesh, *buffers);
        }
    }
}
