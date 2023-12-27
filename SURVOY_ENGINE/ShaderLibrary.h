#pragma once

#include <map>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

namespace BAE {
	class ShaderLibrary
	{
	public:
		ShaderLibrary(int screenW, int screenH);

		void Add(const std::string& key, const char* vertexPath, const char* fragmentPath);
		Shader* GetDefault();
		Shader* Get(const std::string& key);
		GLuint GetID(const std::string& key);
		void Remove(const std::string& key);

	private:
		std::map<std::string, std::unique_ptr<Shader>> _shaders;

		int _screenW;
		int _screenH;
	};
}
