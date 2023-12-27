#include "RendererTileset2D.h"
#include <iostream>

void BAE::RendererTileset2D::Render(
    GLuint shaderProgram,
    GLuint textureID,
    const std::vector<int>& graphic,
    GLfloat x,
    GLfloat y,
    int pxTileWidth,
    int pxTileHeight,
    int mapRows,
    int mapColumns,
    int tilesetColumns,
    int tilesetRows,
    float scale
)
{
    // Initialize buffers
    GLuint _VAO, _VBO;
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4 * graphic.size(), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Activate shader program
    glUseProgram(shaderProgram);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(_VAO);

    // Calculate the total number of vertices required
    std::vector<GLfloat> vertices;
    vertices.reserve(6 * 4 * graphic.size());

    for (size_t i = 0; i < graphic.size(); ++i) {
        int tileIndex = graphic[i]-1;

        int row = tileIndex / tilesetColumns;
        int col = tileIndex % tilesetColumns;

        float uvX = col / static_cast<float>(tilesetColumns);
        float uvY = (tilesetRows - 1 - row) / static_cast<float>(tilesetRows);

        GLfloat u1 = uvX;
        GLfloat v1 = uvY;
        GLfloat u2 = uvX + 1.0f / tilesetColumns;
        GLfloat v2 = uvY + 1.0f / tilesetRows;

        GLfloat xpos = x + (i % mapRows) * pxTileWidth * scale;
        GLfloat ypos = y + (i / mapRows) * pxTileHeight * scale;
        GLfloat w = pxTileWidth * scale;
        GLfloat h = pxTileHeight * scale;

        GLfloat quadVertices[6][4] = {
            { xpos,     ypos + h,   u1, v1 },
            { xpos,     ypos,       u1, v2 },
            { xpos + w, ypos,       u2, v2 },
            { xpos,     ypos + h,   u1, v1 },
            { xpos + w, ypos,       u2, v2 },
            { xpos + w, ypos + h,   u2, v1 }
        };

        vertices.insert(vertices.end(), &quadVertices[0][0], &quadVertices[0][0] + 24);
    }

    // Other rendering code (activating shaders, binding textures, etc.) remains the same

    // Update VBO with all vertices
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());

    // Render all characters in a single draw call
    glDrawArrays(GL_TRIANGLES, 0, 6 * graphic.size());

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Clean up
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}
