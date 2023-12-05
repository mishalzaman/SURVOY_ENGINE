#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"
#include "RenderQuad.h"

namespace ENGINE {
	class Resource
	{
	public:
		Resource();
		~Resource();

		void Create(const std::string& path);
		void SetPosition(glm::vec2 position);
		void SetShader(GLuint id);
		void Draw();

	private:
		GLuint _textureId;
		GLuint _shaderId;
		glm::vec2 _position;
		RenderQuad* _renderQuad;
	};
}
