#pragma once

#include <SDL2/SDL.h>
#include <iostream>

namespace ENGINE {
	class Timer
	{
	public:
		Timer(double updateInterval);
		void BeginFrame();
		bool PhysicsUpdate();

		double DeltaTimeS() { return _deltaTimeS; }
		double DeltaTimeMS() { return _deltaTimeMS; }
	private:
		double _updateInterval; // Fixed update interval in milliseconds
		Uint64 _lastTime;       // Last recorded time
		double _deltaTimeMS;      // Time elapsed since the last frame
		double _deltaTimeS;
		double _accumulator;    // Accumulates elapsed time
	};
}
