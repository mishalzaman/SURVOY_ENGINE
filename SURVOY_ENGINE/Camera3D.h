#pragma once

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

	private:
		const glm::vec3 UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3 FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);

		glm::vec3 _position;
		glm::vec3 _target;
		glm::vec3 _direction;
		glm::vec3 _up;
		glm::vec3 _right;

		float _fov;
	};
}


