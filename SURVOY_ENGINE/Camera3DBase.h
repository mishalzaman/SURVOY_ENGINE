#pragma once

#include <glm/glm.hpp>
#include "CameraHelpers.h"

namespace BAE {
	class Camera3DBase
	{
	public:
		Camera3DBase(float screenWidth, float screenHeight);
		glm::mat4 View();
		glm::mat4 Projection();

		void Position(glm::vec3 position);
		void Forward(glm::vec3 forward);
		virtual void UpdateVectors();

		glm::vec3 Position() { return _Position; }
		glm::vec3 Forward() { return _Forward; }

	protected:
		// cached values
		glm::vec3 _Position;
		glm::vec3 _Forward;
		glm::vec3 _Up;
		glm::vec3 _Right;

		float _Yaw = -90.0f;
		float _Pitch = 0.0f;

		float _MouseX;		 // Relative X offset from last position		
		float _MouseY;		 // Relative Y offset from last position

	private:
		float _screenWidth;
		float _screenHeight;
	};
}
