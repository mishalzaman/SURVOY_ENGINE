#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

namespace BAE {
	class Render3D
	{
	public:
		static void TileSet(
			GLuint shaderProgram,
			GLuint textureID,
			const std::vector<int>& graphic,
			GLfloat x,
			GLfloat y,
			float rotationAngle,
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
