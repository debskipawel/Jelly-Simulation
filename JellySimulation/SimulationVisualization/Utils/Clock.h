#pragma once

#include <chrono>

class Clock
{
	using time = std::chrono::high_resolution_clock;

public:
	Clock() = delete;

	static void Init()
	{
		if (!s_initialized)
		{
			s_initialized = true;
			s_startTimePoint = time::now();
		}
	}

	static float Now()
	{
		if (!s_initialized)
		{
			return 0.0f;
		}

		auto now = time::now();

		return std::chrono::duration<float>(now - s_startTimePoint).count();
	}

protected:
	static bool s_initialized;
	static std::chrono::steady_clock::time_point s_startTimePoint;
};
