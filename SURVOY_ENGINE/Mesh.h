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
		Mesh(std::vector<SVertex> vertices, std::vector<unsigned int> indices, std::vector<STexture> textures);
		void Draw(Shader& shader);

		int NumVertices() { return _vertices.size(); }
	private:
		std::vector<SVertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<STexture> _textures;

		unsigned int _vao, _vbo, _ebo;

		void _setupMesh();
	};
}
