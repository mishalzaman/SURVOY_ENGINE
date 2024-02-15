#pragma once

#include <string>

namespace ECS {
	struct SkyBoxComponent
	{
	public:
		SkyBoxComponent(
			std::string right,
			std::string left,
			std::string top,
			std::string bottom,
			std::string front,
			std::string back
			) :Right(right),
			Left(left),
			Top(top),
			Bottom(bottom),
			Front(front),
			Back(back)
		{}

		std::string Right;
		std::string Left;
		std::string Top;
		std::string Bottom;
		std::string Front;
		std::string Back;
	};
}
#pragma once
