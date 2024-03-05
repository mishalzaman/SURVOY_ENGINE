#include "Timer.h"

ENGINE::Timer::Timer(double updateInterval) :
    _updateInterval(updateInterval),
    _lastTime(SDL_GetPerformanceCounter()),
    _accumulator(0.0)
{
}

void ENGINE::Timer::BeginFrame()
{
    Uint64 now = SDL_GetPerformanceCounter();

    _deltaTimeS = (double)((now - _lastTime) / (double)SDL_GetPerformanceFrequency());
    _deltaTimeMS = (double)((now - _lastTime) * 1000 / (double)SDL_GetPerformanceFrequency());

    // Clamp deltaTime to 60 FPS equivalent
    const double maxDeltaTimeS = 1.0 / 60.0; // Maximum delta time for 60 FPS
    _deltaTimeS = std::min(_deltaTimeS, maxDeltaTimeS);
    _deltaTimeMS = _deltaTimeS * 1000.0; // Recalculate to maintain consistency

    _lastTime = now;
    _accumulator += _deltaTimeMS;
}

bool ENGINE::Timer::FixedUpdate()
{
    if (_accumulator >= _updateInterval) {
        _accumulator -= _updateInterval;
        return true;
    }
    return false;
}
