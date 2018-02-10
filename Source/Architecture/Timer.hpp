#pragma once

//STD
#include <chrono>

using namespace std::chrono_literals;

class Timer
{
public:
	using clock = std::chrono::high_resolution_clock;
	using nanoseconds = std::chrono::nanoseconds;
	using microseconds = std::chrono::microseconds;
	using milliseconds = std::chrono::milliseconds;
	using seconds = std::chrono::seconds;

	using nanoseconds_float = std::chrono::duration<float, nanoseconds::period>;
	using microseconds_float = std::chrono::duration<float, microseconds::period>;
	using milliseconds_float = std::chrono::duration<float, milliseconds::period>;
	using seconds_float = std::chrono::duration<float, seconds::period>;
	
	Timer();

	float getElapsedTime();
	nanoseconds getChronoElapsedTime();

	template <class T>
	float getElapsedTime();

	void restart();

private:
	bool isPaused = false;

	clock::time_point start_time;
};

template <class T>
float Timer::getElapsedTime()
{
	return std::chrono::duration<float, T::period>(clock::now() - start_time).count();
}