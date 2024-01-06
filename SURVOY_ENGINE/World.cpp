#include "World.h"

BAE::World::World(std::vector<int> map, int size):
    _cameraPosition(glm::vec3(0))
{
    _vertices.clear();

    float quadLength = (size*2) / size;

    for (int i = 0; i < map.size(); ++i) {
        int row = i / size;
        int col = i % size;

        float x = -1 + col * quadLength; // X-coordinate
        float z = -1 + row * quadLength; // Z-coordinate (for flat quads on the XZ plane)
        float y = 0; // Y-coordinate (constant if quads are flat on the XZ plane)

        if (map[i] == Tiles::GROUND) {
            // Generate two triangles for each quad
            // UV coordinates for the first tile
            float uMin = 0.0f;
            float uMax = 1.0f / 6.0f; // 1/6th of the texture width
            float vMin = 0.0f;
            float vMax = 1.0f;        // Entire height

            // Triangle 1
            _vertices.push_back({ { x, y, z }, {}, { uMin, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y, z }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x, y, z + quadLength }, {}, { uMin, vMax }, {}, {} });

            // Triangle 2
            _vertices.push_back({ { x + quadLength, y, z }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y, z + quadLength }, {}, { uMax, vMax }, {}, {} });
            _vertices.push_back({ { x, y, z + quadLength }, {}, { uMin, vMax }, {}, {} });
        }

        if (map[i] == Tiles::WALL) {
            // UV coordinates for the second tile
            float uMin = 1.0f / 6.0f;  // Start of the second tile
            float uMax = 2.0f / 6.0f;  // End of the second tile
            float vMin = 0.0f;
            float vMax = 1.0f;         // Full height

            // South facing
            // Triangle 1
            _vertices.push_back({ { x, y, z }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y, z }, {}, { uMin, vMin }, {}, {} });
            _vertices.push_back({ { x, y + quadLength, z }, {}, { uMax, vMax }, {}, {} });

            // Triangle 2
            _vertices.push_back({ { x + quadLength, y, z }, {}, { uMin, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y + quadLength, z }, {}, { uMin, vMax }, {}, {} });
            _vertices.push_back({ { x, y + quadLength, z }, {}, { uMax, vMax }, {}, {} });

            // North facing
            // Triangle 1
            _vertices.push_back({ { x, y, z + quadLength }, {}, { uMin, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y, z + quadLength }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x, y + quadLength, z + quadLength }, {}, { uMin, vMax }, {}, {} });

            // Triangle 2
            _vertices.push_back({ { x + quadLength, y, z + quadLength }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y + quadLength, z + quadLength }, {}, { uMax, vMax }, {}, {} });
            _vertices.push_back({ { x, y + quadLength, z + quadLength }, {}, { uMin, vMax }, {}, {} });

            uMin = 2.0f / 6.0f;  // Start of the second tile
            uMax = 3.0f / 6.0f;  // End of the second tile
            vMin = 0.0f;
            vMax = 1.0f;         // Full height, as the tileset is only one tile high

            // East facing (side of the cube extending along X-axis)
            // Triangle 1
            _vertices.push_back({ { x + quadLength, y, z }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y + quadLength, z }, {}, { uMax, vMax }, {}, {} });
            _vertices.push_back({ { x + quadLength, y + quadLength, z + quadLength }, {}, { uMin, vMax }, {}, {} });

            // Triangle 2
            _vertices.push_back({ { x + quadLength, y + quadLength, z + quadLength }, {}, { uMin, vMax }, {}, {} });
            _vertices.push_back({ { x + quadLength, y, z + quadLength }, {}, { uMin, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y, z }, {}, { uMax, vMin }, {}, {} });

            uMin = 3.0f / 6.0f;  // Start of the second tile
            uMax = 4.0f / 6.0f;  // End of the second tile
            vMin = 0.0f;
            vMax = 1.0f;         // Full height, as the tileset is only one tile high

            // West facing (opposite side to East)
            // Triangle 1
            _vertices.push_back({ { x, y, z }, {}, { uMin, vMin }, {}, {} });
            _vertices.push_back({ { x, y + quadLength, z }, {}, { uMin, vMax }, {}, {} });
            _vertices.push_back({ { x, y + quadLength, z + quadLength }, {}, { uMax, vMax }, {}, {} });

            // Triangle 2
            _vertices.push_back({ { x, y + quadLength, z + quadLength }, {}, { uMax, vMax }, {}, {} });
            _vertices.push_back({ { x, y, z + quadLength }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x, y, z }, {}, { uMin, vMin }, {}, {} });

            uMin = 4.0f / 6.0f;  // Start of the second tile
            uMax = 5.0f / 6.0f;  // End of the second tile
            vMin = 0.0f;
            vMax = 1.0f;         // Full height, as the tileset is only one tile high

            // Top facing
            // Triangle 1
            _vertices.push_back({ { x, y + quadLength, z }, {}, { uMin, vMax }, {}, {} });
            _vertices.push_back({ { x + quadLength, y + quadLength, z }, {}, { uMax, vMax }, {}, {} });
            _vertices.push_back({ { x, y + quadLength, z + quadLength }, {}, { uMin, vMin }, {}, {} });

            // Triangle 2
            _vertices.push_back({ { x + quadLength, y + quadLength, z }, {}, { uMax, vMax }, {}, {} });
            _vertices.push_back({ { x + quadLength, y + quadLength, z + quadLength }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x, y + quadLength, z + quadLength }, {}, { uMin, vMin }, {}, {} });
        }

        if (map[i] == Tiles::CAMERA) {
            // Ground

            // Generate two triangles for each quad
            // UV coordinates for the first tile
            float uMin = 0.0f;
            float uMax = 1.0f / 6.0f; // 1/6th of the texture width
            float vMin = 0.0f;
            float vMax = 1.0f;        // Entire height

            // Triangle 1
            _vertices.push_back({ { x, y, z }, {}, { uMin, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y, z }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x, y, z + quadLength }, {}, { uMin, vMax }, {}, {} });

            // Triangle 2
            _vertices.push_back({ { x + quadLength, y, z }, {}, { uMax, vMin }, {}, {} });
            _vertices.push_back({ { x + quadLength, y, z + quadLength }, {}, { uMax, vMax }, {}, {} });
            _vertices.push_back({ { x, y, z + quadLength }, {}, { uMin, vMax }, {}, {} });

            // Camera Position

            float cX = x + (quadLength / 2);
            float cY = y + (quadLength / 2);
            float cZ = z + (quadLength / 2);

            _cameraPosition = glm::vec3(cX, cY, cZ);
        }
    }
}
