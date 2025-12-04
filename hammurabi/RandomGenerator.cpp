#include "include/RandomGenerator.h"
#include <random>

RandomGenerator::RandomGenerator(const GameParams& params, unsigned int seed)
    : rng_(seed),
      params_(params)
{
}

bool RandomGenerator::getRandomBool(int chancePercent)
{
    return std::uniform_int_distribution<int>(1, 100)(rng_) <= chancePercent;
}

int RandomGenerator::getRandomInRange(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng_);
}

int RandomGenerator::getRandomLandPrice()
{
    return getRandomInRange(params_.min_land_price, params_.max_land_price);
}

bool RandomGenerator::checkPlague()
{
    return getRandomBool(params_.plague_chance);
}
