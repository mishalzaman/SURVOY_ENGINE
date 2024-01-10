#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cmath>
#include "Defaults.h"
#include "CameraHelpers.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

namespace BAE {
	class Camera3D
	{
	public:
		Camera3D(glm::vec3 position, float screenWidth, float screenHeight);
		glm::mat4 ViewMat4();
		glm::mat4 ProjectionMat4();

		void SetPosition(glm::vec3 position);
		void SetForward(glm::vec3 forward);
		void Update();

	private:
		// cached values
		glm::vec3 _position;
		glm::vec3 _forward;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _worldUp;

		float _screenWidth;
		float _screenHeight;

		void _cacheVectors();
	};
}


