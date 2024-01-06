#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include "Camera3D.h"
#include "Defaults.h"
#include "SVertex.h"

namespace BAE {
	class Renderer3D
	{
	public:
		Renderer3D(GLuint textureID0, std::vector<SVertex> vertices);
		~Renderer3D();

		void render(
			BAE::Shader& shader,
			BAE::Camera3D& camera,
			glm::vec3 position
		);

	private:
		unsigned int _vbo;
		unsigned int _vao;
		GLuint _textureID0;

		std::vector<SVertex> _vertices;

		void _initializeBuffers();
	};
}
