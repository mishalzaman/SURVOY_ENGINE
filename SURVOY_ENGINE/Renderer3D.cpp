#include "Renderer3D.h"
#include <iostream>

BAE::Renderer3D::Renderer3D(GLuint textureID):
    _vbo(0),
    _vao(0),
    _textureID0(textureID)
{
    //_vertices = {
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    //};

    _testGenerateMap();
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

    // time
    shader.setFloat("time", SDL_GetTicks());

    // projection
    shader.setMat4("projection", camera.Projection());
    
    // view
    shader.setMat4("view", camera.View());

    // model
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, position);
    //float angle = 20.0f * i;
    //model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", model);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureID0);

    // render
    glBindVertexArray(_vao);
    glDrawArrays(
        GL_TRIANGLES,
        0,
        _vertices.size() / 5
    );
}

void BAE::Renderer3D::_initializeBuffers()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices.size(), _vertices.data(), GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void BAE::Renderer3D::_testGenerateMap()
{
    std::vector<int> map = {
        0, 0, 1, 0,
        0, 1, 1, 1,
        1, 1, 0, 0,
        0, 1, 0, 1
    };
    int rows = 4;
    int cols = 4;

    //tilesheet u v
    float G_U = 0;
    float G_V = 0.2;

    // Clear any existing vertices
    _vertices.clear();

    // Dimensions of each quad
    float quadWidth = 2.0f / cols;
    float quadHeight = 2.0f / rows;
    float quadDepth = 1.0f; // Depth for quads extending along z-axis

    for (int i = 0; i < map.size(); ++i) {
        int row = i / cols;
        int col = i % cols;

        // Bottom left corner of the quad
        float x = -1 + col * quadWidth;
        float y = -1 + row * quadHeight;

        // quad
        if (map[i] == 0) {
            // Generate two triangles for each quad

            // UV coordinates for the first tile
            float uMin = 0.0f;
            float uMax = 1.0f / 5.0f; // 1/5th of the texture width
            float vMin = 0.0f;
            float vMax = 1.0f;        // Entire height

            // Triangle 1
            _vertices.insert(_vertices.end(), { x, y, 0, uMin, vMin }); // Bottom left
            _vertices.insert(_vertices.end(), { x + quadWidth, y, 0, uMax, vMin }); // Bottom right
            _vertices.insert(_vertices.end(), { x, y + quadHeight, 0, uMin, vMax }); // Top left

            // Triangle 2
            _vertices.insert(_vertices.end(), { x + quadWidth, y, 0, uMax, vMin }); // Bottom right
            _vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, 0, uMax, vMax }); // Top right
            _vertices.insert(_vertices.end(), { x, y + quadHeight, 0, uMin, vMax }); // Top left
        }

        // cube
        else if (map[i] == 1) {
            // Generate a quad using the z-axis

            // Face South
            // UV coordinates for the second tile
            float uMin = 1.0f / 5.0f;  // Start of the second tile
            float uMax = 2.0f / 5.0f;  // End of the second tile
            float vMin = 0.0f;
            float vMax = 1.0f;         // Full height, as the tileset is only one tile high

            // First triangle of the cube
            _vertices.insert(_vertices.end(), { x, y, 0, uMin, vMin }); // Bottom left (front)
            _vertices.insert(_vertices.end(), { x + quadWidth, y, 0, uMax, vMin }); // Bottom right (front)
            _vertices.insert(_vertices.end(), { x + quadWidth, y, quadWidth, uMax, vMax }); // Bottom right (back)

            // Second triangle of the cube
            _vertices.insert(_vertices.end(), { x + quadWidth, y, quadWidth, uMax, vMax }); // Bottom right (back)
            _vertices.insert(_vertices.end(), { x, y, quadWidth, uMin, vMax }); // Bottom left (back)
            _vertices.insert(_vertices.end(), { x, y, 0, uMin, vMin }); // Bottom left (front)


            // Face East
            // First triangle of the cube
            _vertices.insert(_vertices.end(), { x + quadWidth, y, 0, 0.0f, 0.0f }); // Bottom right (front)
            _vertices.insert(_vertices.end(), { x + quadWidth, y, quadWidth, 1.0f, 0.0f }); // Bottom right (back)
            _vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, quadWidth, 1.0f, 1.0f }); // Top right (back)

            // Second triangle of the cube
            _vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, quadWidth, 1.0f, 1.0f }); // Top right (back)
            _vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, 0, 0.0f, 1.0f }); // Top right (front)
            _vertices.insert(_vertices.end(), { x + quadWidth, y, 0, 0.0f, 0.0f }); // Bottom right (front)

            // Face North
            // First triangle of the cube
            _vertices.insert(_vertices.end(), { x, y, quadWidth, 0.0f, 0.0f }); // Bottom left (back)
            _vertices.insert(_vertices.end(), { x, y + quadHeight, quadWidth, 0.0f, 1.0f }); // Top left (back)
            _vertices.insert(_vertices.end(), { x, y + quadHeight, 0, 1.0f, 1.0f }); // Top left (front)

            // Second triangle of the cube
            _vertices.insert(_vertices.end(), { x, y + quadHeight, 0, 1.0f, 1.0f }); // Top left (front)
            _vertices.insert(_vertices.end(), { x, y, 0, 1.0f, 0.0f }); // Bottom left (front)
            _vertices.insert(_vertices.end(), { x, y, quadWidth, 0.0f, 0.0f }); // Bottom left (back)

            // Face West
            // First triangle of the cube
            _vertices.insert(_vertices.end(), { x, y, quadWidth, 0.0f, 0.0f }); // Bottom left (back)
            _vertices.insert(_vertices.end(), { x, y, 0, 1.0f, 0.0f }); // Bottom left (front)
            _vertices.insert(_vertices.end(), { x, y + quadHeight, 0, 1.0f, 1.0f }); // Top left (front)

            // Second triangle of the cube
            _vertices.insert(_vertices.end(), { x, y + quadHeight, 0, 1.0f, 1.0f }); // Top left (front)
            _vertices.insert(_vertices.end(), { x, y + quadHeight, quadWidth, 0.0f, 1.0f }); // Top left (back)
            _vertices.insert(_vertices.end(), { x, y, quadWidth, 0.0f, 0.0f }); // Bottom left (back)

            // Top
            _vertices.insert(_vertices.end(), { x, y, quadWidth, 0.0f, 0.0f }); // Bottom left
            _vertices.insert(_vertices.end(), { x + quadWidth, y, quadWidth, 1.0f, 0.0f }); // Bottom right
            _vertices.insert(_vertices.end(), { x, y + quadHeight, quadWidth, 0.0f, 1.0f }); // Top left

            _vertices.insert(_vertices.end(), { x + quadWidth, y, quadWidth, 1.0f, 0.0f }); // Bottom right
            _vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, quadWidth, 1.0f, 1.0f }); // Top right
            _vertices.insert(_vertices.end(), { x, y + quadHeight, quadWidth, 0.0f, 1.0f }); // Top left
        }

    }
}

//// Face East
//// First triangle of the cube
//_vertices.insert(_vertices.end(), { x + quadWidth, y, 0, 0.0f, 0.0f }); // Bottom right (front)
//_vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, 0, 1.0f, 0.0f }); // Bottom right (back)
//_vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, quadWidth, 1.0f, 1.0f }); // Top right (back)
//
//// Second triangle of the cube
//_vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, quadWidth, 1.0f, 1.0f }); // Top right (back)
//_vertices.insert(_vertices.end(), { x + quadWidth, y + quadHeight, 0, 0.0f, 1.0f }); // Top right (front)
//_vertices.insert(_vertices.end(), { x + quadWidth, y, 0, 0.0f, 0.0f }); // Bottom right (front)