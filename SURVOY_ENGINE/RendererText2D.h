#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <vector>

namespace ENGINE {
	class RendererText2D
	{
	public:
		static void Render(
			GLuint shaderProgram,
			GLuint textureID,
			const std::string& text, 
			GLfloat x, GLfloat y,
			glm::vec3 color,
			float scale
		);
	};
}
