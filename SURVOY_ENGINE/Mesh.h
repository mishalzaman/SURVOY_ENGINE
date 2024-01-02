#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "Shader.h"

namespace BAE {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct TextureM {
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureM> textures);
		void Draw(Shader& shader);
	private:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<TextureM> _textures;

		unsigned int _vao, _vbo, _ebo;

		void _setupMesh();
	};
}
