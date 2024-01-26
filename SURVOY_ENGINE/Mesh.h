#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "SVertex.h"
#include "STexture.h"

namespace BAE {
	class Mesh
	{
	public:
		Mesh(
			std::vector<SVertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<STexture> textures,
			glm::mat4 transformation,
			std::string name
		);
		void Draw(Shader& shader);

		glm::mat4 Transformation() { return _transformation; };
		glm::vec3 Position();
		int NumVertices() { return _vertices.size(); }
		std::vector<SVertex> Vertices() { return _vertices; }
		std::vector<unsigned int> Indices() { return _indices; }
		std::vector<STexture> Textures() { return _textures; }
		std::string Name() { return _name; }
	private:
		std::vector<SVertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<STexture> _textures;

		unsigned int _vao, _vbo, _ebo;
		glm::mat4 _transformation;

		std::string _name;

		void _setupMesh();
	};
}
