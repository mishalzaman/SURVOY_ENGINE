#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "Camera3DBase.h"

const float C_SPEED = 2.f;
const float C_MOUSE_SENSITIVITY = 20.f;

namespace BAE {
	class CameraFreeLook : public Camera3DBase
	{
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
