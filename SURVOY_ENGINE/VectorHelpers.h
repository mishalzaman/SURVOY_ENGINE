#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

const float NEAR = 0.1f;
const float FAR = 1000.0f;

namespace BAE {
	class VectorHelpers
	{
	public:
		static glm::mat4 ViewMat4(
			const glm::vec3 position,
			const glm::vec3 forward,
			const glm::vec3 up
		);

		static glm::mat4 ProjectionMat4(
			const float screenW, 
			const float screenH,
			const float fovDegrees
		);

		static glm::mat4 LookAtTargetViewMat4(
			const glm::vec3 target,
			const glm::vec3 position
		);

		static glm::vec3 ForwardVec3(
			const float yaw,
			const float pitch
		);

		static glm::vec3 RightVec3(
			const glm::vec3 forward
		);

		static glm::vec3 UpVec3(
			const glm::vec3 forward,
			const glm::vec3 right
		);
	};
}
