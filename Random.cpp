#include "Random.hpp"
#include <random>
#include <ctime>
#include <cassert>

std::mt19937& getGenerator()
{
    static std::mt19937 generator(static_cast<unsigned long>(std::time(nullptr)));
    return generator;
}

int random(int min, int max)
{
    assert(min <= max);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(getGenerator());
}

unsigned int random(unsigned int min, unsigned int max)
{
    assert(min <= max);
    std::uniform_int_distribution<unsigned int> distribution(min, max);
    return distribution(getGenerator());
}

float random(float min, float max)
{
    assert(min <= max);
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(getGenerator());
}

float randomDev(float middle, float deviation)
{
    assert(deviation >= 0.f);
    return random(middle - deviation, middle + deviation);
}

void setRandomSeed(unsigned long seed)
{
    getGenerator().seed(seed);
}
