#include "MeshStaticBuffersSystem.h"

ECS::MeshStaticBuffersSystem::MeshStaticBuffersSystem(EntityManager& entityManager):
	_entityManager(entityManager)
{
}

ECS::MeshStaticBuffersSystem::~MeshStaticBuffersSystem()
{
}

void ECS::MeshStaticBuffersSystem::Load()
{
    // Retrieve entity IDs by tag
    std::vector<int> entities = _entityManager.getByTags("Mesh");

    for (int entityId : entities) {
        // Retrieve the components required for rendering
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);

        if (mesh && buffers) {
            // Initialize the buffers for the mesh
            glGenVertexArrays(1, &buffers->VAO);
            glGenBuffers(1, &buffers->VBO);
            glGenBuffers(1, &buffers->EBO);

            glBindVertexArray(buffers->VAO);
            glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);

            glBufferData(GL_ARRAY_BUFFER, mesh->Vertices.size() * sizeof(SVertex), &mesh->Vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->Indices.size() * sizeof(unsigned int), &mesh->Indices[0], GL_STATIC_DRAW);

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
    }
}

void ECS::MeshStaticBuffersSystem::Unload()
{
    std::vector<int> entities = _entityManager.getByTags("Mesh");

    for (int entityId : entities) {
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        if (buffers) {
            // Delete VAO, VBO, and EBO
            glDeleteVertexArrays(1, &buffers->VAO);
            glDeleteBuffers(1, &buffers->VBO);
            glDeleteBuffers(1, &buffers->EBO);
        }
    }
}
