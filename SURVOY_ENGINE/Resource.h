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
		void SetScale(int s) { _scale = s; }

		GLuint GetTextureID() { return _textureId; };
		GLuint GetShaderID() { return _shaderId; };
		glm::vec2 GetPosition() { return _position; };
		int GetWidth() { return _width; };
		int GetHeight() { return _height; };
		int GetChannel() { return _channels; };
		float GetScale() { return _scale; };

	private:
		GLuint _textureId;
		GLuint _shaderId;
		glm::vec2 _position;
		RenderQuad* _renderQuad;
		float _scale;
		int _width;
		int _height;
		int _channels;
	};
}
