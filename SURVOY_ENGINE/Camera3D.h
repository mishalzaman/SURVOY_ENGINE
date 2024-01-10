#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cmath>
#include "Defaults.h"

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
		glm::vec3 Position() { return _position; }
		glm::vec3 Velocity() { return _velocity; }
		float Yaw() { return glm::radians(_yaw); }

		void UpdatePosition(glm::vec3 position) { _position = position; }

		void TankMovement(SDL_Event& event, float deltaTime);
		void LookAtTarget(glm::vec3 target);

		void UpdateCamera(glm::vec3 position, glm::vec3 forward);

	private:
		glm::vec3 _position;
		glm::vec3 _forward;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _worldUp;

		float _yaw;
		float _pitch;

		float _movementSpeed;
		glm::vec3 _velocity;  // Current velocity of the camera
		float _acceleration;  // Acceleration rate of the camera
		float _maxSpeed;      // Maximum speed of the camera

		float _screenWidth;
		float _screenHeight;

		void _updateCameraVectors();
	};
}


