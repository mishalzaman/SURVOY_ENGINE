#include "MeshRenderSystem.h"

ECS::MeshRenderSystem::MeshRenderSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager):
    _eventManager(eventManager), _entityManager(entityManager), _physics(physics)
{
    _defaultShader = std::make_unique<Shader>("lighting_3d_vertex.glsl", "lighting_3d_fragment.glsl");
    _eventManager.subscribe(this);
}

ECS::MeshRenderSystem::~MeshRenderSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::MeshRenderSystem::onNotify(const Event& event)
{
    const auto* cameraEvent = dynamic_cast<const CameraViewProjectionEvent*>(&event);

    if (cameraEvent) {
        _view = cameraEvent->getViewMatrix();
        _projection = cameraEvent->getProjectionMatrix();
    }
}

void ECS::MeshRenderSystem::Load() {
    auto& entities = _entityManager.getEntityComponentIndices(); // Access the entity-component mapping

    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

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
    auto& entities = _entityManager.getEntityComponentIndices(); // Access the entity-component mapping

    // Second pass to render each entity
    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

        // Retrieve the components required for rendering
        ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        ECS::TexturesComponent* textures = _entityManager.getComponent<ECS::TexturesComponent>(entityId);

        if (transform && mesh && buffers && textures) {
            _render(*transform, *mesh, *buffers, *textures);
        }
    }
}

void ECS::MeshRenderSystem::Unload() {
    auto& entities = _entityManager.getEntityComponentIndices(); // Access the entity-component mapping

    for (const auto& entityPair : entities) {
        int entityId = entityPair.first;

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


void ECS::MeshRenderSystem::_render(const TransformComponent& transform, const MeshComponent& mesh, const BuffersComponent& buffers, const TexturesComponent& textures)
{
    _defaultShader->use();
    _defaultShader->setMat4("projection", _projection);
    _defaultShader->setMat4("view", _view);
    _defaultShader->setVec3("lightPos", glm::vec3(8, 10, 8));
    _defaultShader->setVec3("viewPos", glm::vec3(0, 1, 0));
    _defaultShader->setVec3("lightColor", glm::vec3(1, 1, 1));

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

        _defaultShader->setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures.Textures[i].id);

        _defaultShader->setMat4("model", transform.transformation);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(buffers.VAO);
    glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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