#pragma once

#include "Camera3DBase.h"

namespace BAE {
	class CameraFollow : public Camera3DBase
	{
	const float C_SPEED = 2.f;
	const float C_MOUSE_SENSITIVITY = 20.f;

	public:
		CameraFollow(float screenWidth, float screenHeight);
		void Update(glm::vec3 target, float deltaTime);
		void SetMouseOffsets(float x, float y);

		float _acceleration;

		glm::vec3 _target;
		float _distanceToTarget;

		float _mouseX;
		float _mouseY;

	private:
		void _move(float deltaTime);
		void _orbit(float deltaTime);
	};
}
