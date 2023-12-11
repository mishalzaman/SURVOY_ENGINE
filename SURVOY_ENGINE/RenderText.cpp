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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4 * text.length(), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Activate shader program
	glUseProgram(shaderProgram);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(_VAO);

	// bitmap font properties
	const int charW = 16;
	const int charH = 16;
	const int rows = 16;
	const int columns = 32;

	std::vector<GLfloat> vertices;
	vertices.reserve(6 * 4 * text.length()); // 6 vertices per character, 4 values per vertex

	int initialX = x;

	for (auto c = text.begin(); c != text.end(); c++) {
		if (*c == '\n') {
			// Reset x to initial value and move y down by one character height (plus optional additional spacing)
			x = initialX; // initialX should be stored before the loop starts
			y += charH * scale; // lineSpacing can be 0 or some positive value for extra spacing
			continue;
		}

		int charIndex = *c - 32;
		int row = charIndex / columns;
		int col = charIndex % columns;

		float uvX = col / static_cast<float>(columns);
		float uvY = (rows - 1 - row) / static_cast<float>(rows);

		GLfloat xpos = x;
		GLfloat ypos = y;
		GLfloat w = charW * scale;
		GLfloat h = charH * scale;

		GLfloat u1 = uvX;
		GLfloat v1 = uvY;
		GLfloat u2 = uvX + 1.0f / columns;
		GLfloat v2 = uvY + 1.0f / rows;

		GLfloat quadVertices[6][4] = {
			{ xpos,     ypos + h,   u1, v1 },
			{ xpos,     ypos,       u1, v2 },
			{ xpos + w, ypos,       u2, v2 },
			{ xpos,     ypos + h,   u1, v1 },
			{ xpos + w, ypos,       u2, v2 },
			{ xpos + w, ypos + h,   u2, v1 }
		};

		vertices.insert(vertices.end(), &quadVertices[0][0], &quadVertices[0][0] + 24);

		// Advance cursors for next glyph
		x += w * scale; // Adjust spacing as needed
	}

	// Update VBO with all vertices
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());

	// Render all characters in a single draw call
	glDrawArrays(GL_TRIANGLES, 0, 6 * text.length());

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Clean up
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}
