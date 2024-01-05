#pragma once

#include <vector>

namespace BAE {
	class World
	{
	public:
		World(std::vector<int> map, int size);

		std::vector<float> StaticVertices() { return _vertices; }
	private:
		std::vector<float> _vertices;
	};
}
