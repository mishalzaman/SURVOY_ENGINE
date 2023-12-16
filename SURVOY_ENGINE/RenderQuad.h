#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace BAE {
	class RenderQuad
	{
	public:
		static void Render(
			GLuint shaderProgram,
			GLuint textureID,
			GLfloat x, GLfloat y, GLfloat width, GLfloat height,
			float scale
		);

		static void Render3d(
			GLuint shaderProgram,
			GLuint textureID,
			GLfloat x, GLfloat y, GLfloat width, GLfloat height,
			float scale,
			float rotationAngle
		);
	};
}


