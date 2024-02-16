#include "SkyBoxSystem.h"

ECS::SkyBoxSystem::SkyBoxSystem(EntityManager& entityManager, EventManager& eventManager):
	_entityManager(entityManager), _eventManager(eventManager), _VAO(0), _VBO(0), _view(glm::mat4(1)), _projection(glm::mat4(1)), _cube{
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
	}
{
    _eventManager.subscribe(this);
}

ECS::SkyBoxSystem::~SkyBoxSystem()
{
    _eventManager.unsubscribe(this);
}

void ECS::SkyBoxSystem::onNotify(const Event& event)
{
    const auto* cameraEvent = dynamic_cast<const CameraViewProjectionEvent*>(&event);

    if (cameraEvent) {
        _view = cameraEvent->getViewMatrix();
        _projection = cameraEvent->getProjectionMatrix();
    }
}

void ECS::SkyBoxSystem::Load()
{
    // Cube buffers
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VAO);
    glBufferData(GL_ARRAY_BUFFER, _cube.size() * sizeof(float), _cube.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Initialize textures
    int e = _entityManager.getByTags("SkyBox")[0];

    // Retrieve the components required for rendering
    ECS::SkyBoxComponent* skybox = _entityManager.getComponent<ECS::SkyBoxComponent>(e);

    if (skybox) {
        std::vector<std::string> faces{
            skybox->Right,
            skybox->Left,
            skybox->Top,
            skybox->Bottom,
            skybox->Front,
            skybox->Back
        };

        STexture texture;
        ENGINE::FileLoader::TextureSkyBox(texture, faces);
        _texture = texture.id;
    }
    
    // set texture
    e = _entityManager.getByTags("SkyBoxShader")[0];

    ECS::ProgramComponent* program = _entityManager.getComponent<ECS::ProgramComponent>(e);

    if (program) {
        program->Program.use();
        program->Program.setInt("skybox", 0);
    }
}

void ECS::SkyBoxSystem::UpdatePrePhysics()
{
}

void ECS::SkyBoxSystem::Update(float deltaTime)
{
}

void ECS::SkyBoxSystem::UpdatePostPhysics()
{
}

void ECS::SkyBoxSystem::Render()
{
    int e = _entityManager.getByTags("SkyBoxShader")[0];

    ECS::ProgramComponent* program = _entityManager.getComponent<ECS::ProgramComponent>(e);

    if (program) {

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        program->Program.use();
        glm::mat4 view = glm::mat4(1);
        view = glm::mat4(glm::mat3(_view));
        program->Program.setMat4("view", view);
        program->Program.setMat4("projection", _projection);
        // skybox cube
        glBindVertexArray(_VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    }
}

void ECS::SkyBoxSystem::Unload()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}
