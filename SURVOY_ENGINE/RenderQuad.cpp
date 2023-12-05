#include "RenderQuad.h"

void ENGINE::RenderQuad::Render(
    GLuint shaderProgram,
    GLuint textureID,
    GLfloat x, GLfloat y, GLfloat width, GLfloat height,
    glm::vec3 color,
    float scale
) {
    // Initialize buffers
    GLuint _VAO, _VBO;
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Activate shader program
    glUseProgram(shaderProgram);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0); // Activate the texture unit first
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0); // Set the sampler to use Texture Unit 0

    glBindVertexArray(_VAO);

    // Define quad vertices (including texture coordinates)
    GLfloat xpos = x;
    GLfloat ypos = y;
    GLfloat w = width * scale;
    GLfloat h = height * scale;

    GLfloat vertices[6][4] = {
        // positions   // texture coords
        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }
    };

    // Render quad
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0); // Optionally unbind the texture
}
