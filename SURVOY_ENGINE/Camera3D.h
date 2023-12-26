#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Defaults.h"

namespace BAE {
	class Camera3D
	{
	public:
		Camera3D();

		glm::mat4 View();
		glm::mat4 Projection();

		// setters
		void SetPosition(glm::vec3 val) { _position = val; }
		void SetTarget(glm::vec3 val) { _target = val; }

		void UpdateOrbit(SDL_Event& event, float deltaTime);

	private:
		const glm::vec3 UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3 FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);

		glm::vec3 _position;
		glm::vec3 _target;
		glm::vec3 _direction;
		glm::vec3 _up;
		glm::vec3 _right;

		float _fov;

		float _horizontalAngle = 0.0f; // Horizontal angle towards the target
		float _verticalAngle = 0.0f;   // Vertical angle
		float _orbitDistance = 3.0f;    // Distance from the target

		// Orbit speed factors
		float _orbitSpeedH = 2.0f;
		float _orbitSpeedV = 2.0f;
	};
}


