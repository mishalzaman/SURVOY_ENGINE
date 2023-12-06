#pragma once

#include <map>
#include <string>
#include <memory>
#include "Shader.h"

namespace ENGINE {
	class ShaderLibrary
	{
	public:
		void Add(const std::string& key, const char* vertexPath, const char* fragmentPath);
		Shader* GetDefault();
		Shader* Get(const std::string& key);
		GLuint GetID(const std::string& key);
		void Remove(const std::string& key);

	private:
		std::map<std::string, std::unique_ptr<Shader>> _shaders;
	};
}
