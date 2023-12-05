#pragma once

#include "Resource.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "RenderQuad.h"

namespace ENGINE {
	class Scene
	{
	public:
		Scene(int screenW, int screenH);
		~Scene();
		
		/*==============================================
		RESOURCE
		==============================================*/
		void AddResource(const std::string& key, const std::string& path);
		void DestroyResource(const std::string& key);
		Resource* GetResource(const std::string& key);

		/*==============================================
		RENDER
		==============================================*/
		void Draw();

	private:
		std::map<std::string, std::unique_ptr<Resource>> _resources;
		float _screenW;
		float _screenH;

		Shader* _defaultShader;
		void _setDefaultShader();
	};
}
