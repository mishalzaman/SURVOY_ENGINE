#include "Renderer3D.h"
#include <iostream>

BAE::Renderer3D::Renderer3D(GLuint textureID0, std::vector<SVertex> vertices) :
    _vbo(0),
    _vao(0),
    _textureID0(textureID0)
{
    _vertices = vertices;
    _initializeBuffers();
}

BAE::Renderer3D::~Renderer3D()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void BAE::Renderer3D::render(
    BAE::Shader& shader,
    BAE::Camera3D& camera,
    glm::vec3 position
)
{
    shader.use();
    shader.setInt("texture1", 0);

    // model
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, position);
    //float angle = 20.0f * i;
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureID0);

    // render
    glBindVertexArray(_vao);
    glDrawArrays(
        GL_TRIANGLES,
        0,
        _vertices.size()
    );
}

void BAE::Renderer3D::_initializeBuffers()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(SVertex), &_vertices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, TexCoords));

    glBindVertexArray(0);
}
