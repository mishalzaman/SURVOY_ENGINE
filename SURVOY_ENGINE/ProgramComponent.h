#pragma once

#include <GL/glew.h>
#include "Shader.h"

using namespace BAE;

namespace ECS {
	struct ProgramComponent
	{
	public:
		ProgramComponent(Shader& program) :
			Program(program) {};

		Shader& Program;
	};
}

