#pragma once

#include <vector>

namespace BAE {
	class GridMeshGenerator
	{
	public:
		GridMeshGenerator(std::vector<int> map, int size);

		std::vector<float> StaticVertices() { return _vertices; }
		std::vector<float> PlayerVertices() { return _playerVertices; }
	private:
		std::vector<float> _vertices;
		std::vector<float> _playerVertices;
	};
}
