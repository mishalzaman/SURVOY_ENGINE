#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <vector>

namespace ENGINE {
	class RendererTileset2D
	{
	public:
		static void Render(
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
		);
	};
}


