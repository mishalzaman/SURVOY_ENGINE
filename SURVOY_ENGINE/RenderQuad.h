#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

namespace ENGINE {
	class RenderQuad
	{
	public:
		static void Render(
			GLuint shaderProgram,
			GLuint textureID,
			GLfloat x, GLfloat y, GLfloat width, GLfloat height,
			glm::vec3 color,
			float scale
		);
	};
}


