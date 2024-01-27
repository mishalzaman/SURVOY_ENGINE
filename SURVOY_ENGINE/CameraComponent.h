#pragma once

#include <glm/glm.hpp>
#include "Component.h"

namespace ECS {
	class CameraComponent : public Component
	{
	public:
		// cached values
		glm::mat4 View;
		glm::mat4 Projection;

		glm::vec3 Position;
		glm::vec3 Forward;
		glm::vec3 Up;
		glm::vec3 Right;

		float Yaw;
		float Pitch;

		float MouseX;		 // Relative X offset from last position		
		float MouseY;		 // Relative Y offset from last position

		float ScreenWidth;
		float ScreenHeight;

		CameraComponent(
			glm::vec3 position,
			float screenWidth,
			float screenHeight
		) : View(glm::mat4(0)),
			Projection(glm::mat4(0)),
			Position(position),
			Forward(glm::vec3(0, 0, 1)),
			Up(glm::vec3(0, 1, 0)),
			Right(glm::vec3(0)),
			Yaw(-90.0f),
			Pitch(0.0f),
			MouseX(0.0f),
			MouseY(0.0f),
			ScreenWidth(screenWidth),
			ScreenHeight(screenHeight) {};
	};
}
