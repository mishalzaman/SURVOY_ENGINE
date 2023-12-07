#pragma once

#include <SDL2/SDL.h>

namespace BAE {
	class Timer
	{
	public:
		Timer(double updateInterval);
		void BeginFrame();
		bool ShouldUpdate();

		double DeltaTime() { return _deltaTime; }
	private:
		double _updateInterval; // Fixed update interval in milliseconds
		Uint64 _lastTime;       // Last recorded time
		double _deltaTime;      // Time elapsed since the last frame
		double _accumulator;    // Accumulates elapsed time
	};
}
