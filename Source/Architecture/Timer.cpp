#include "Timer.hpp"

Timer::Timer()
{
	restart();
}

float Timer::getElapsedTime()
{
	return getElapsedTime<seconds>();
}

Timer::nanoseconds Timer::getChronoElapsedTime()
{
	return clock::now() - start_time;
}

void Timer::restart()
{
	start_time = clock::now();
}