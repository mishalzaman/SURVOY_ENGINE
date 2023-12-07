#pragma once

#include <string>

namespace Material {
	class Material
	{
	public:
		Material(const std::string& shaderName, const std::string& textureName);

	private:
		const std::string& _shaderName;
		const std::string& _textureName;
	};
}
