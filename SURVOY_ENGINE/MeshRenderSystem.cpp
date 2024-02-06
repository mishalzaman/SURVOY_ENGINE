#include "MeshRenderSystem.h"

ECS::MeshRenderSystem::MeshRenderSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager):
    _eventManager(eventManager), _entityManager(entityManager), _physics(physics)
{
    _eventManager.subscribe(this);
}

ECS::MeshRenderSystem::~MeshRenderSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::MeshRenderSystem::onNotify(const Event& event)
{
    const auto* cameraEvent = dynamic_cast<const CameraViewProjectionEvent*>(&event);
    const auto* cameraPositionEvent = dynamic_cast<const CameraPositionEvent*>(&event);

    if (cameraEvent) {
        std::vector<int> entities = _entityManager.getByTags("Mesh, Player Mesh");

        for (int entityId : entities) {
            ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(entityId);

            if (matrices) {
                matrices->View = cameraEvent->getViewMatrix();
                matrices->Projection = cameraEvent->getProjectionMatrix();
            }
        }
    }

    if (cameraPositionEvent) {
        std::vector<int> entities = _entityManager.getByTags("Mesh, Player Mesh");

        for (int entityId : entities) {
            ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);

            if (transform) {
                _cameraPosition = transform->position;
            }
        }
    }
}

void ECS::MeshRenderSystem::Load() {
    // Retrieve entity IDs by tag
    std::vector<int> entities = _entityManager.getByTags("Mesh, Player Mesh");

    for (int entityId : entities) {
        // Retrieve the components required for rendering
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);

        if (mesh && buffers) {
            // Initialize the buffers for the mesh
            _initBuffers(*mesh, *buffers);
        }
    }
}

void ECS::MeshRenderSystem::Renders() {
    std::vector<int> entities = _entityManager.getByTags("Mesh, Player Mesh");

    for (int entityId : entities) {
        // Retrieve the components required for rendering
        ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        ECS::TexturesComponent* textures = _entityManager.getComponent<ECS::TexturesComponent>(entityId);
        ECS::CameraMatricesComponent* matrices = _entityManager.getComponent<ECS::CameraMatricesComponent>(entityId);

        if (transform && mesh && buffers && textures && matrices) {
            _render(*transform, *mesh, *buffers, *textures, *matrices);
        }
    }
}

void ECS::MeshRenderSystem::Unload() {
    std::vector<int> entities = _entityManager.getByTags("Mesh, Player Mesh");

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

void ECS::MeshRenderSystem::Update()
{
}

void ECS::MeshRenderSystem::Update(float deltaTime)
{
}


void ECS::MeshRenderSystem::_render(
    const TransformComponent& transform,
    const MeshComponent& mesh,
    const BuffersComponent& buffers,
    const TexturesComponent& textures,
    const CameraMatricesComponent& matrices
)
{
    glm::vec3 viewPosition = glm::vec3(0);

    std::vector<int> entitiesC = _entityManager.getByTag("Camera");
    for (int entityId : entitiesC) {
        ECS::OrientationComponent* orientation = _entityManager.getComponent<ECS::OrientationComponent>(entityId);

        if (orientation) {
            viewPosition = orientation->Position;
        }
    }

    // get shader
    std::vector<int> entities = _entityManager.getByTag("Shader 3d");

    for (int entityId : entities) {
        ECS::ProgramComponent* shader = _entityManager.getComponent<ECS::ProgramComponent>(entityId);
        if (shader) {
            shader->Program.use();
            shader->Program.setVec3("lightPos", glm::vec3(2, 20, 6));
            shader->Program.setVec3("viewPos", viewPosition);
            shader->Program.setVec3("lightColor", glm::vec3(0.7, 0.7, 0.7));
            shader->Program.setMat4("projection", matrices.Projection);
            shader->Program.setMat4("view", matrices.View);
            shader->Program.setInt("texture1", 0);
            shader->Program.setMat4("model", transform.transformation);

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
    }
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