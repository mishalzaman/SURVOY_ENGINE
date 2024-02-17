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
	_ebo(0),
	_vertices(vertices),
	_indices(indices),
	_textures(textures),
	_transformation(transformation),
	_name(name)
{
}

glm::vec3 ENGINE::Mesh::Position()
{
	glm::vec3 position;
	position.x = _transformation[3].x;
	position.y = _transformation[3].y;
	position.z = _transformation[3].z;

	return position;
}
