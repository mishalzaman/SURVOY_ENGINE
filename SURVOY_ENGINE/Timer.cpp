#include "Timer.h"

BAE::Timer::Timer(double updateInterval):
    _updateInterval(updateInterval),
    _lastTime(SDL_GetPerformanceCounter()),
    _accumulator(0.0)
{
}

void BAE::Timer::BeginFrame()
{
    Uint64 now = SDL_GetPerformanceCounter();
    _deltaTime = (double)((now - _lastTime) * 1000 / (double)SDL_GetPerformanceFrequency());
    _lastTime = now;
    _accumulator += _deltaTime;
}

bool BAE::Timer::ShouldUpdate()
{
    if (_accumulator >= _updateInterval) {
        _accumulator -= _updateInterval;
        return true;
    }
    return false;
}