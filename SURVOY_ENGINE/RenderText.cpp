#include "RenderText.h"

void BAE::RenderText::Render(
	GLuint shaderProgram,
	GLuint textureID,
	const std::string& text,
	GLfloat x,
	GLfloat y,
	glm::vec3 color,
	float scale
)
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

	// bitmap font properties
	int charW = 16;
	int charH = 16;
	int rows = 16;
	int columns = 32;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {

		int charIndex = *c - 32;
		int row = charIndex / columns;
		int col = charIndex % columns;

		//std::cout << " row " << row << " col " << col << std::endl;

		float uvX = col / static_cast<float>(columns);
		float uvY = (rows - 1 - row) / static_cast<float>(rows);

		//std::cout << " uvX " << uvX << " uvY " << uvY << std::endl;

		GLfloat xpos = x;
		GLfloat ypos = y;
		GLfloat w = charW * scale;
		GLfloat h = charH * scale;

		GLfloat u1 = uvX;
		GLfloat v1 = uvY;
		GLfloat u2 = uvX + 1.0f / columns;
		GLfloat v2 = uvY + 1.0f / rows;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   u1, v1 },
			{ xpos,     ypos,       u1, v2 },
			{ xpos + w, ypos,       u2, v2 },

			{ xpos,     ypos + h,   u1, v1 },
			{ xpos + w, ypos,       u2, v2 },
			{ xpos + w, ypos + h,   u2, v1 }
		};

		// Render quad (similar to RenderQuad class)
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Advance cursors for next glyph
		x += w;
	}


	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0); // Optionally unbind the texture


	//// bitmap font properties
	//int charW = 16;
	//int charH = 16;
	//int rows = 32;
	//int columns = 16;

	//// Initialize buffers
	//GLuint _VAO, _VBO;
	//glGenVertexArrays(1, &_VAO);
	//glGenBuffers(1, &_VBO);

	//glBindVertexArray(_VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//// Activate shader program
	//glUseProgram(shaderProgram);

	//// Bind the texture
	//glActiveTexture(GL_TEXTURE0); // Activate the texture unit first
	//glBindTexture(GL_TEXTURE_2D, textureID);

	//glBindVertexArray(_VAO);

	//// Iterate over each character
	//std::string::const_iterator c;
	//for (c = text.begin(); c != text.end(); c++) {
	//	int charIndex = *c - 32;
	//	int row = charIndex / columns;
	//	int col = charIndex % columns;

	//	float uvX = col / static_cast<float>(columns);
	//	float uvY = row / static_cast<float>(rows);

	//	GLfloat xpos = x;
	//	GLfloat ypos = y;
	//	GLfloat w = charW * scale;
	//	GLfloat h = charH * scale;

	//	GLfloat u1 = uvX;
	//	GLfloat v1 = uvY;
	//	GLfloat u2 = uvX + 1.0f / columns;
	//	GLfloat v2 = uvY + 1.0f / rows;

	//	GLfloat vertices[6][4] = {
	//		{ xpos,     ypos + h,   u1, v1 },
	//		{ xpos,     ypos,       u1, v2 },
	//		{ xpos + w, ypos,       u2, v2 },

	//		{ xpos,     ypos + h,   u1, v1 },
	//		{ xpos + w, ypos,       u2, v2 },
	//		{ xpos + w, ypos + h,   u2, v1 }
	//	};

	//	// Render quad (similar to RenderQuad class)
	//	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);

	//	// Advance cursors for next glyph
	//	x += w;
	//}

	//glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0); // Optionally unbind the texture
}
