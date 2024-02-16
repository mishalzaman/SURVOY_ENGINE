#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Shader.h"

namespace ENGINE {
	class Grid
	{
	public:
		Grid();
		void render(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos);
	private:
		int _gridSize;
		int _numLines;
		GLuint _vao;
		void _setupAxis();
		std::unique_ptr<ENGINE::Shader> _shader;
	};
}
