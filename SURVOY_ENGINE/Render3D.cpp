#include "Render3D.h"

void BAE::Render3D::TileSet(GLuint shaderProgram, GLuint textureID, const std::vector<int>& graphic, GLfloat x, GLfloat y, float rotationAngle, int pxTileWidth, int pxTileHeight, int mapRows, int mapColumns, int tilesetColumns, int tilesetRows, float scale)
{
    // Create the model matrix for your object (translation, rotation, etc.)
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, 0.0f)); // Translate
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1024 * scale, 1024 * scale, 1.0f)); // Scale

    // Calculate the projection matrix
    float aspect = static_cast<float>(1024) / static_cast<float>(768); // Calculate the aspect ratio based on window dimensions
    float nearPlane = 0.1f; // Set the near clipping plane (adjust as needed)
    float farPlane = 1000.0f; // Set the far clipping plane (adjust as needed)

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), aspect, nearPlane, farPlane);

    // Create the view matrix for your camera
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 900.0f), // Move the camera further away (adjust the Z value)
        glm::vec3(0.0f, 0.0f, 0.0f), // Look-at position (center)
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    );

    // Combine the matrices: projection * view * model
    glm::mat4 finalMatrix = projectionMatrix * viewMatrix * modelMatrix;

    // Activate shader program
    glUseProgram(shaderProgram);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalMatrix));
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Calculate the total number of vertices required
    std::vector<GLfloat> vertices;
    vertices.reserve(6 * 5 * graphic.size());

    for (size_t i = 0; i < graphic.size(); ++i) {
        int tileIndex = graphic[i] - 1;

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

        GLfloat quadVertices[6][5] = {
            { xpos,     ypos + h,   0.0f, u1, v1 },
            { xpos,     ypos,       0.0f, u1, v2 },
            { xpos + w, ypos,       0.0f, u2, v2 },
            { xpos,     ypos + h,   0.0f, u1, v1 },
            { xpos + w, ypos,       0.0f, u2, v2 },
            { xpos + w, ypos + h,   0.0f, u2, v1 }
        };


        vertices.insert(vertices.end(), &quadVertices[0][0], &quadVertices[0][0] + 6 * 5);
    }

    // Initialize buffers
    GLuint _VAO, _VBO;
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);


    // Assuming your texture coordinates are stored in the same buffer as positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // The texture coordinates may start at offset 3 floats (assuming 3 floats for position)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));



    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Other rendering code (activating shaders, binding textures, etc.) remains the same

    // Bind the vertex array and draw
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * graphic.size());

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Clean up
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}
