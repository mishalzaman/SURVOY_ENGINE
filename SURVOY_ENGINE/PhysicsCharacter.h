#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

const float TURN_SPEED = 2.0f;

namespace BAE {
	class PhysicsCharacter
	{
	public:
		PhysicsCharacter(glm::vec3 position);
		void MoveForward(float deltaTime);
		void MoveBackward(float deltaTime);
		void TurnLeft(float deltaTime);
		void TurnRight(float deltaTime);
		void Decelerate();
		void Update();

		void Move(float deltaTime);

		// getters
		glm::vec3 V3Position() { return _v3Position; }
		glm::vec3 V3Forward() { return _v3Forward; }
		glm::vec3 V3Velocity() { return _v3Velocity; }
		float fYaw() { return _fYaw; }
		float fPitch() { return _fPitch; }

		// setters
		void SetV3Yaw(float yaw) { _fYaw = yaw; }
		void SetV3Position(glm::vec3 position) { _v3Position = position; }
		void SetForwardV3(glm::vec3 forward) { _v3Forward = forward; }

	private:
		// positioning
		glm::vec3			_v3Position;
		glm::vec3			_v3Forward;

		// orientation
		float				_fYaw;
		float				_fPitch;

		// movement
		float				_fMovementSpeed;
		glm::vec3			_v3Velocity;  // Current velocity of the camera
		float				_fAcceleration;  // Acceleration rate of the camera
		float				_fMaxSpeed;      // Maximum speed of the camera
		float				_fMass;

		void _setForward();
		void _clampVelocity();
	};
}
