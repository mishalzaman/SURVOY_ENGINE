#pragma once

#include <vector>
#include <string>
#include "SVertex.h"

using namespace ENGINE;

namespace ECS {
	struct MeshComponent
	{
	public:
		MeshComponent(
			const std::string& name,
			std::vector<SVertex> vertices,
			std::vector<unsigned int> indices
		) : Name(name), Vertices(vertices), Indices(indices) {}

		std::string Name;
		std::vector<SVertex> Vertices;
		std::vector<unsigned int> Indices;
	};
}