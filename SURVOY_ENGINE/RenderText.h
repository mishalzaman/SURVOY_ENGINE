#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>

namespace ENGINE {
	class RenderText
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
