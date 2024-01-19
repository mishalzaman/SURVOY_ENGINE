#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "Camera3DBase.h"

namespace BAE {
	class CameraFreeLook : public Camera3DBase
	{
	const float SPEED = 2.f;
	const float MOUSE_SENSITIVITY = 20.f;

	public:
		CameraFreeLook(float screenWidth, float screenHeight);
		void Update(float deltaTime);
		void SetMouseOffsets(float x, float y);

		float _acceleration;

		float _mouseX;
		float _mouseY;

	private:
		void _move(float deltaTime);
		void _mouseLook(float deltaTime);
	};
}
