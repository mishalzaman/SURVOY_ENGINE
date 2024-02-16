#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "STexture.h"
#include "SVertex2D.h"

namespace ENGINE {
	class RendererQuad2D
	{
	public:
		RendererQuad2D(STexture& texture, GLuint shaderID, int positionX, int positionY, float scale);
		void Render();

	private:
		int _textureWidth, _textureHeight, _positionX, _positionY;
		unsigned int _textureID;
		float _scale;
		GLuint _vao, _vbo, _shaderID;

		void _initBuffers();
		void _initVertices();

		std::vector<SVertex2D> _vertices;
	};
}


