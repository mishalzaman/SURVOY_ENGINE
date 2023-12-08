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

	private:
		float _screenW;
		float _screenH;
	};
}
