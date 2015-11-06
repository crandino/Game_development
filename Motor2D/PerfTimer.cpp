// ----------------------------------------------------
// PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 PerfTimer::frequency = 0;

// ---------------------------------------------
PerfTimer::PerfTimer()
{
	frequency = SDL_GetPerformanceFrequency();
	start();
}

// ---------------------------------------------
void PerfTimer::start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double PerfTimer::readMs() const
{
	return readTicks() / (frequency * 1000.0f);
}

// ---------------------------------------------
uint64 PerfTimer::readTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}


