#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "SVertex.h"

namespace BAE {
	enum Tiles {
		GROUND,
		WALL,
		CAMERA
	};

	class World
	{
	public:
		World(std::vector<int> map, int size);

		std::vector<SVertex> Vertices() { return _vertices; }
		glm::vec3 CameraPosition() { return _cameraPosition; }
	private:
		std::vector<SVertex> _vertices;
		glm::vec3 _cameraPosition;

	};
}
