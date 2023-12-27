#pragma once

#include <vector>

namespace BAE {
	class MeshGenerator
	{
	public:
		MeshGenerator(std::vector<int> map, int size);

		std::vector<float> GetVertices() { return _vertices; }
	private:
		std::vector<float> _vertices;
	};
}
