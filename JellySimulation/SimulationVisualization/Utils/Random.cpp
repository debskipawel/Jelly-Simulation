#include "pch.h"
#include "Random.h"

Random::Random()
{
    std::random_device rd;
    m_engine = std::mt19937(rd());
    m_distribution = std::uniform_real_distribution<>(0.0f, 1.0f);
}

Random::Random(int seed)
{
    m_engine = std::mt19937(seed);
    m_distribution = std::uniform_real_distribution<>(0.0f, 1.0f);
}

float Random::Next()
{
    return static_cast<float>(m_distribution(m_engine));
}

float Random::Next(float min, float max)
{
    if (min > max)
    {
        return 0.0f;
    }

    return Next() * (max - min) + min;
}
