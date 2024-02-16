#include "Timer.h"

ENGINE::Timer::Timer(double updateInterval):
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
    _lastTime = now;
    _accumulator += _deltaTimeMS;
}

bool ENGINE::Timer::PhysicsUpdate()
{
    if (_accumulator >= _updateInterval) {
        _accumulator -= _updateInterval;
        return true;
    }
    return false;
}
