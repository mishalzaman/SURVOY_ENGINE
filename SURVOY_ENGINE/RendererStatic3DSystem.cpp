#include "RendererStatic3DSystem.h"

// Macro to automatically include file and line information
#define GL_CHECK(stmt) do { \
        stmt; \
        CheckGLError(#stmt, __FILE__, __LINE__); \
    } while (0)

void CheckGLError(const char* statement, const char* file, int line) {
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        const char* errorString = "UNKNOWN_ERROR";
        switch (error) {
        case GL_INVALID_ENUM:                  errorString = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 errorString = "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             errorString = "GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                errorString = "GL_STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               errorString = "GL_STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 errorString = "GL_OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errorString = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            // Add more cases as needed
        }
        std::cerr << "OpenGL error " << error << " (" << errorString << ") in " << file << " at line " << line << ": " << statement << std::endl;
        error = glGetError(); // Check for subsequent errors
    }
}

ECS::RendererStatic3DSystem::RendererStatic3DSystem(EntityManager& entityManager, Physics& physics, EventManager& eventManager):
    _eventManager(eventManager),
    _entityManager(entityManager),
    _physics(physics)
{
    _eventManager.subscribe(this);
}

ECS::RendererStatic3DSystem::~RendererStatic3DSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::RendererStatic3DSystem::onNotify(const Event& event)
{
    const auto* cameraEvent = dynamic_cast<const CameraViewProjectionEvent*>(&event);
    const auto* cameraPositionEvent = dynamic_cast<const CameraPositionEvent*>(&event);

    if (cameraEvent) {
        _view = cameraEvent->getViewMatrix();
        _projection = cameraEvent->getProjectionMatrix();
    }

    if (cameraPositionEvent) {
        _cameraPosition = cameraPositionEvent->getPosition();
    }
}

void ECS::RendererStatic3DSystem::Load() {
    // Retrieve entity IDs by tag
    std::vector<int> entities = _entityManager.getByTags("Mesh");

    for (int entityId : entities) {
        // Retrieve the components required for rendering
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);

        if (mesh && buffers) {
            // Initialize the buffers for the mesh
            GL_CHECK(glGenVertexArrays(1, &buffers->VAO));
            GL_CHECK(glGenBuffers(1, &buffers->VBO));
            GL_CHECK(glGenBuffers(1, &buffers->EBO));

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

void ECS::RendererStatic3DSystem::Render() {
    std::vector<int> entities = _entityManager.getByTags("Mesh");

    int e = _entityManager.getByTag("DefaultShader")[0];
    ECS::ProgramComponent* shader = _entityManager.getComponent<ECS::ProgramComponent>(e);

    if (shader) {
        shader->Program.use();
        shader->Program.setVec3("lightPos", glm::vec3(0, 16, 0));
        shader->Program.setVec3("viewPos", _cameraPosition);
        shader->Program.setVec3("lightColor", glm::vec3(0.9, 0.9, 0.9));
        shader->Program.setMat4("projection", _projection);
        shader->Program.setMat4("view", _view);
        shader->Program.setInt("texture1", 0);
    }
    else {
        std::cout << "could not load shader" << std::endl;
    }

    for (int entityId : entities) {
        // Retrieve the components required for rendering
        ECS::TransformComponent* transform = _entityManager.getComponent<ECS::TransformComponent>(entityId);
        ECS::MeshComponent* mesh = _entityManager.getComponent<ECS::MeshComponent>(entityId);
        ECS::BuffersComponent* buffers = _entityManager.getComponent<ECS::BuffersComponent>(entityId);
        ECS::TexturesComponent* textures = _entityManager.getComponent<ECS::TexturesComponent>(entityId);



        if (transform && mesh && buffers && textures && shader) {
            shader->Program.setMat4("model", transform->Transformation);
            _render(*transform, *mesh, *buffers, *textures);
        }
    }
}

void ECS::RendererStatic3DSystem::Unload() {
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

void ECS::RendererStatic3DSystem::UpdatePrePhysics()
{
}

void ECS::RendererStatic3DSystem::Update(float deltaTime)
{
}

void ECS::RendererStatic3DSystem::UpdatePostPhysics()
{
}


void ECS::RendererStatic3DSystem::_render(
    const TransformComponent& transform,
    const MeshComponent& mesh,
    const BuffersComponent& buffers,
    const TexturesComponent& textures
)
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
