#pragma once

#include "Camera3DBase.h"

namespace BAE {
	class CameraFollow : public Camera3DBase
	{
	const float MOUSE_SENSITIVITY = 20.f;

	public:
		CameraFollow(float screenWidth, float screenHeight);
		void			Update(glm::vec3 target, float deltaTime);
		void			SetMouseOffsets(float x, float y);

		float			_acceleration;

		glm::vec3		_target;
		float			_distanceToTarget;

	private:
		void			_move(float deltaTime);
		void			_orbit(float deltaTime);
	};
}
