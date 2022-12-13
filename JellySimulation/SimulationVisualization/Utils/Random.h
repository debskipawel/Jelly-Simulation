#pragma once

#include <random>

class Random
{
public:
	Random();
	Random(int seed);

	float Next();
	float Next(float min, float max);

private:
	std::mt19937 m_engine;
	std::uniform_real_distribution<> m_distribution;
};
