#include "RendererQuad2D.h"
#include <chrono>

BAE::RendererQuad2D::RendererQuad2D(STexture& texture, GLuint shaderID, int positionX, int positionY, float scale):
    _vao(0),
    _vbo(0),
    _textureWidth(texture.width),
    _textureHeight(texture.height),
    _positionX(positionX),
    _positionY(positionY),
    _scale(scale),
    _textureID(texture.id),
    _shaderID(shaderID)
{
    _initVertices();
    _initBuffers();
}

void BAE::RendererQuad2D::Render() {
    // Activate shader program
    glUseProgram(_shaderID);

    // Set shader uniforms here (if any)

    // Bind the texture
    glActiveTexture(GL_TEXTURE0); // Activate the texture unit first
    glBindTexture(GL_TEXTURE_2D, _textureID);

    // Bind the VAO (which also binds the VBO)
    glBindVertexArray(_vao);

    // Draw the quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Optional: Unbind the VAO and texture to prevent unintended state changes
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void BAE::RendererQuad2D::_initBuffers() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(SVertex2D), &_vertices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (void*)0);

    // texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (void*)offsetof(SVertex2D, TexCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void BAE::RendererQuad2D::_initVertices()
{
    GLfloat xpos = _positionX;
    GLfloat ypos = _positionY;
    GLfloat w = _textureWidth * _scale;
    GLfloat h = _textureHeight * _scale;

    _vertices = {
        // positions       // texture coords
        { glm::vec2(xpos, ypos + h), glm::vec2(0.0f, 0.0f) },
        { glm::vec2(xpos, ypos),     glm::vec2(0.0f, 1.0f) },
        { glm::vec2(xpos + w, ypos), glm::vec2(1.0f, 1.0f) },

        { glm::vec2(xpos, ypos + h), glm::vec2(0.0f, 0.0f) },
        { glm::vec2(xpos + w, ypos), glm::vec2(1.0f, 1.0f) },
        { glm::vec2(xpos + w, ypos + h), glm::vec2(1.0f, 0.0f) }
    };
}
