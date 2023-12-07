#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "RenderQuad.h"

namespace BAE {
	class Scene
	{
	public:
		Scene(int screenW, int screenH);
		~Scene();

		/*==============================================
		RENDER
		==============================================*/
		void Draw();

	private:
		float _screenW;
		float _screenH;

		Shader* _defaultShader;
		void _setDefaultShader();
	};
}

/*
- List of shaders
- List of textures
- Material

Image
CreateObject("name", Material)
GetObject("name")->SetPosition()
				 ->SetScale()

CreateObject("text", Material, "hello")
*/