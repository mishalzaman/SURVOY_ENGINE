#pragma once

namespace ECS {
	struct NearFarPlanesComponent
	{
	public:
		NearFarPlanesComponent(float near, float far) :
			Near(near), Far(far) {};

		float Near;
		float Far;
	};
}
