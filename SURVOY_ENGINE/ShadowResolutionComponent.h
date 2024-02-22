#pragma once

namespace ECS {
	struct ShadowResolutionComponent
	{
		ShadowResolutionComponent(float width, float height) :
			Width(width), Height(height) {};

		float Width;
		float Height;
	};
}
