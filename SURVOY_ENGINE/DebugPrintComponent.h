#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>

struct PrintText {
	std::string Text;
	glm::vec3 Colour = glm::vec3(1,1,1);
};

namespace ECS {
	struct DebugPrintComponent
	{
	public:
		DebugPrintComponent() {}

		std::vector<PrintText> PrintTexts;
	};
}
