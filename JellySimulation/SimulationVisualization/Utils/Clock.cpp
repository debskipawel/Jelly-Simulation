#include "pch.h"
#include "Clock.h"

bool Clock::s_initialized = false;
std::chrono::steady_clock::time_point Clock::s_startTimePoint;
