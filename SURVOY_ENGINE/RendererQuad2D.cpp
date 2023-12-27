#include "RendererQuad2D.h"
#include <chrono>

void BAE::RendererQuad2D::Render(GLuint shaderProgram, GLuint textureID, GLfloat x, GLfloat y, GLfloat width, GLfloat height, float scale)
{
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

void BAE::RendererQuad2D::Render3d(GLuint shaderProgram, GLuint textureID, GLfloat x, GLfloat y, GLfloat width, GLfloat height, float scale, float rotationAngle)
{
    // Initialize buffers
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Calculate the projection matrix
    float aspect = static_cast<float>(1024) / static_cast<float>(768); // Calculate the aspect ratio based on window dimensions
    float nearPlane = 0.1f; // Set the near clipping plane (adjust as needed)
    float farPlane = 1000.0f; // Set the far clipping plane (adjust as needed)

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), aspect, nearPlane, farPlane);

    // Create the model matrix for your object (translation, rotation, etc.)
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Apply your transformations here (translation, rotation, scaling, etc.)
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, 0.0f)); // Translate
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis
    modelMatrix = glm::scale(modelMatrix, glm::vec3(width * scale, height * scale, 1.0f)); // Scale

    // Create the view matrix for your camera
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 900.0f), // Move the camera further away (adjust the Z value)
        glm::vec3(0.0f, 0.0f, 0.0f), // Look-at position (center)
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    );

    // Combine the matrices: projection * view * model
    glm::mat4 finalMatrix = projectionMatrix * viewMatrix * modelMatrix;



    // get matrix's uniform location and set matrix
    // Activate shader program
    glUseProgram(shaderProgram);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalMatrix));
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    // render container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Render quad
    //glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0); // Optionally unbind the texture
}