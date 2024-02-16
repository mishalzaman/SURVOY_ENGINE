#include "Grid.h"

ENGINE::Grid::Grid():
	_gridSize(100),
	_numLines(0),
	_vao(0)
{
	_shader = std::make_unique<ENGINE::Shader>("grid_vertex.glsl", "grid_fragment.glsl");
	_setupAxis();
}

void ENGINE::Grid::render(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos)
{
	_shader->use();
	_shader->setMat4("projection", projection);
	_shader->setMat4("view", view);
	_shader->setVec3("colour", glm::vec3(0.15, 0.15, 0.15));
	_shader->setVec3("viewPos", viewPos);

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINES, 0, _numLines);
	glBindVertexArray(0);
}

void ENGINE::Grid::_setupAxis()
{
	GLuint VBO;

	std::vector<glm::vec3> gridData;
	int gridIncrementer;

	// Y Axis
	gridData.push_back(glm::vec3(0, -_gridSize, 0));
	gridData.push_back(glm::vec3(0, _gridSize, 0));

	// X Axis
	gridIncrementer = -_gridSize;
	for (int i = 0; i <= _gridSize * 2; i++) {
		gridData.push_back(glm::vec3(-_gridSize, 0, gridIncrementer));
		gridData.push_back(glm::vec3(_gridSize, 0, gridIncrementer));

		gridIncrementer++;
	}

	// Z Axis
	gridIncrementer = -_gridSize;
	for (int i = 0; i <= _gridSize * 2; i++) {
		gridData.push_back(glm::vec3(gridIncrementer, 0, -_gridSize));
		gridData.push_back(glm::vec3(gridIncrementer, 0, _gridSize));

		gridIncrementer++;
	}

	_numLines = gridData.size();

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &_vao);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(glm::vec3), &gridData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}
