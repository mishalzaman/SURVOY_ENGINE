#include "Mesh.h"
#include <iostream>

ENGINE::Mesh::Mesh(
	std::vector<SVertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<STexture> textures,
	glm::mat4 transformation,
	std::string name
):
	_vao(0),
	_vbo(0),
	_ebo(0)
{
	this->_vertices = vertices;
	this->_indices = indices;
	this->_textures = textures;
	this->_transformation = transformation;
	this->_name = name;

	_setupMesh();
}

void ENGINE::Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = _textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, _textures[i].id);

		shader.setMat4("model", _transformation);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

glm::vec3 ENGINE::Mesh::Position()
{
	glm::vec3 position;
	position.x = _transformation[3].x;
	position.y = _transformation[3].y;
	position.z = _transformation[3].z;

	return position;
}

void ENGINE::Mesh::_setupMesh()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(SVertex), &_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, TexCoords));

	glBindVertexArray(0);
}
