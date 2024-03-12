#pragma once

#include <glm/glm.hpp>

namespace ECS {
	struct MovementAttributesComponent
	{
	public:
		glm::vec3 HorizontalVelocity;
		glm::vec3 VerticalVelocity;
		glm::vec3 Velocity;
		glm::vec3 Gravity;
		glm::vec3 Displacement;
		float Speed;
		float Acceleration;
		float Deceleration;
		float TurnRate;

		MovementAttributesComponent(
			float speed,
			float acceleration,
			float deceleration,
			float turnRate,
			glm::vec3 gravity
		) :
			HorizontalVelocity(glm::vec3(0)),
			VerticalVelocity(glm::vec3(0)),
			Displacement(glm::vec3(0)),
			Velocity(glm::vec3(0)),
			Speed(speed),
			Acceleration(acceleration),
			Deceleration(deceleration),
			TurnRate(turnRate),
			Gravity(gravity) {}
	};
}
