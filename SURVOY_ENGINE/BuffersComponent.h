#pragma once

#include <GL/glew.h>
#include <vector>
#include "Component.h"
#include "SVertex.h"

using namespace BAE;

namespace ECS {
	class BuffersComponent : public Component
	{
	public:
		BuffersComponent(
			std::vector<SVertex> vertices,
			std::vector<unsigned int> indices
		);

		unsigned int VAO, VBO, EBO;
	};
}
