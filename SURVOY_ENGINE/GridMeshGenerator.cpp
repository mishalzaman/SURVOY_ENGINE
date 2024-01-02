#include "GridMeshGenerator.h"

BAE::GridMeshGenerator::GridMeshGenerator(std::vector<int> map, int size)
{
	_vertices.clear();

	float quadLength = 2.0f / size;

	for (int i = 0; i < map.size(); ++i) {
		int row = i / size;
		int col = i % size;

        float x = -1 + col * quadLength; // X-coordinate
        float z = -1 + row * quadLength; // Z-coordinate (for flat quads on the XZ plane)
        float y = 0; // Y-coordinate (constant if quads are flat on the XZ plane)


		if (map[i] == 0) {
            // Generate two triangles for each quad
            // UV coordinates for the first tile
            float uMin = 0.0f;
            float uMax = 1.0f / 6.0f; // 1/5th of the texture width
            float vMin = 0.0f;
            float vMax = 1.0f;        // Entire height

            // Triangle 1
            _vertices.insert(_vertices.end(), { x, 0, z, uMin, vMin }); // Bottom left
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z, uMax, vMin }); // Bottom right
            _vertices.insert(_vertices.end(), { x, 0, z + quadLength, uMin, vMax }); // Top left

            // Triangle 2
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z, uMax, vMin }); // Bottom right
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z + quadLength, uMax, vMax }); // Top right
            _vertices.insert(_vertices.end(), { x, 0, z + quadLength, uMin, vMax }); // Top left
		}


        if (map[i] == 1) {
            // UV coordinates for the second tile
            float uMin = 1.0f / 6.0f;  // Start of the second tile
            float uMax = 2.0f / 6.0f;  // End of the second tile
            float vMin = 0.0f;
            float vMax = 1.0f;         // Full height, as the tileset is only one tile high

            // South facing
            // Triangle 1
            _vertices.insert(_vertices.end(), { x, 0, z, uMax, vMin }); // Bottom left (front)
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z, uMin, vMin }); // Bottom right (front)
            _vertices.insert(_vertices.end(), { x, quadLength, z, uMax, vMax }); // Top left (front)

            // Triangle 2
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z, uMin, vMin }); // Bottom right (front)
            _vertices.insert(_vertices.end(), { x + quadLength, quadLength, z, uMin, vMax }); // Top right (front)
            _vertices.insert(_vertices.end(), { x, quadLength, z, uMax, vMax }); // Top left (front)

            // North facing
            // Triangle 1
            _vertices.insert(_vertices.end(), { x, 0, z + quadLength, uMin, vMin }); // Bottom left
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z + quadLength, uMax, vMin }); // Bottom right
            _vertices.insert(_vertices.end(), { x, y + quadLength, z + quadLength, uMin, vMax }); // Top left

            // Triangle 2
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z + quadLength, uMax, vMin }); // Bottom right
            _vertices.insert(_vertices.end(), { x + quadLength, y + quadLength, z + quadLength, uMax, vMax }); // Top right
            _vertices.insert(_vertices.end(), { x, y + quadLength, z + quadLength, uMin, vMax }); // Top left

            uMin = 2.0f / 6.0f;  // Start of the second tile
            uMax = 3.0f / 6.0f;  // End of the second tile
            vMin = 0.0f;
            vMax = 1.0f;         // Full height, as the tileset is only one tile high

            // East facing (side of the cube extending along X-axis)
            // Triangle 1
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z, uMax, vMin }); // Bottom right (front)
            _vertices.insert(_vertices.end(), { x + quadLength, y + quadLength, z, uMax, vMax }); // Top right (front)
            _vertices.insert(_vertices.end(), { x + quadLength, y + quadLength, z + quadLength, uMin, vMax }); // Top right (back)

            // Triangle 2
            _vertices.insert(_vertices.end(), { x + quadLength, y + quadLength, z + quadLength, uMin, vMax }); // Top right (back)
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z + quadLength, uMin, vMin }); // Bottom right (back)
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z, uMax, vMin }); // Bottom right (front)

            uMin = 3.0f / 6.0f;  // Start of the second tile
            uMax = 4.0f / 6.0f;  // End of the second tile
            vMin = 0.0f;
            vMax = 1.0f;         // Full height, as the tileset is only one tile high

            // West facing (opposite side to East)
            // Triangle 1
            _vertices.insert(_vertices.end(), { x, 0, z, uMin, vMin }); // Bottom left (front)
            _vertices.insert(_vertices.end(), { x, y + quadLength, z, uMin, vMax }); // Top left (front)
            _vertices.insert(_vertices.end(), { x, y + quadLength, z + quadLength, uMax, vMax }); // Top left (back)

            // Triangle 2
            _vertices.insert(_vertices.end(), { x, y + quadLength, z + quadLength, uMax, vMax }); // Top left (back)
            _vertices.insert(_vertices.end(), { x, 0, z + quadLength, uMax, vMin }); // Bottom left (back)
            _vertices.insert(_vertices.end(), { x, 0, z, uMin, vMin }); // Bottom left (front)

            uMin = 4.0f / 6.0f;  // Start of the second tile
            uMax = 5.0f / 6.0f;  // End of the second tile
            vMin = 0.0f;
            vMax = 1.0f;         // Full height, as the tileset is only one tile high

            // Top facing
            // Triangle 1
            _vertices.insert(_vertices.end(), { x, quadLength, z, uMin, vMax }); // Bottom left (near)
            _vertices.insert(_vertices.end(), { x + quadLength, quadLength, z, uMax, vMax }); // Bottom right (near)
            _vertices.insert(_vertices.end(), { x, quadLength, z + quadLength, uMin, vMin }); // Top left (far)

            // Triangle 2
            _vertices.insert(_vertices.end(), { x + quadLength, quadLength, z, uMax, vMax }); // Bottom right (near)
            _vertices.insert(_vertices.end(), { x + quadLength, quadLength, z + quadLength, uMax, vMin }); // Top right (far)
            _vertices.insert(_vertices.end(), { x, quadLength, z + quadLength, uMin, vMin }); // Top left (far)
        }

        if (map[i] == 2) {
            // Generate two triangles for each quad
            // UV coordinates for the first tile
            float uMin = 0.0f;
            float uMax = 1.0f / 6.0f; // 1/5th of the texture width
            float vMin = 0.0f;
            float vMax = 1.0f;        // Entire height

            // Create the ground
            // Triangle 1
            _vertices.insert(_vertices.end(), { x, 0, z, uMin, vMin }); // Bottom left
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z, uMax, vMin }); // Bottom right
            _vertices.insert(_vertices.end(), { x, 0, z + quadLength, uMin, vMax }); // Top left

            // Triangle 2
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z, uMax, vMin }); // Bottom right
            _vertices.insert(_vertices.end(), { x + quadLength, 0, z + quadLength, uMax, vMax }); // Top right
            _vertices.insert(_vertices.end(), { x, 0, z + quadLength, uMin, vMax }); // Top left

            // Create the player
            uMin = 5.0f / 6.0f;  // Start of the second tile
            uMax = 6.0f / 6.0f;  // End of the second tile
            vMin = 0.0f;
            vMax = 1.0f;         // Full height, as the tileset is only one tile high

            quadLength = quadLength * 0.8;
            x += quadLength * 0.1;
            y = 0.01;
            z += quadLength * 0.1;
            
            _playerVertices.clear();

            // Create the ground
            // Triangle 1
            _playerVertices.insert(_playerVertices.end(), { x, y, z, uMin, vMax }); // Bottom left
            _playerVertices.insert(_playerVertices.end(), { x + quadLength, y, z, uMax, vMax }); // Bottom right
            _playerVertices.insert(_playerVertices.end(), { x, y, z + quadLength, uMin, vMin }); // Top left

            // Triangle 2
            _playerVertices.insert(_playerVertices.end(), { x + quadLength, y, z, uMax, vMax }); // Bottom right
            _playerVertices.insert(_playerVertices.end(), { x + quadLength, y, z + quadLength, uMax, vMin }); // Top right
            _playerVertices.insert(_playerVertices.end(), { x, y, z + quadLength, uMin, vMin }); // Top left
        }
	}
}
