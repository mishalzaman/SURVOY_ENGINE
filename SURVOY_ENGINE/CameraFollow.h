#pragma once

#include "Camera3DBase.h"

namespace BAE {
	class CameraFollow : public Camera3DBase
	{
	const float MOUSE_SENSITIVITY = 20.f;

	public:
		static constexpr float DISTANCE_TO_FOLLOW = 6.f;

		CameraFollow(float screenWidth, float screenHeight);
		void			Update(glm::vec3 target, float deltaTime);
		void			SetMouseOffsets(float x, float y);

		float			Yaw() { return _Yaw; }
	private:
		float			_acceleration;

		glm::vec3		_target;
		float			_distanceToTarget;

		void			_move();
		void			_orbit(float deltaTime);
	};
}
