#pragma once
#include "GameParams.h"
#include <random>

class RandomGenerator
{
private:
    std::mt19937 rng_;
    GameParams params_;

public:
    explicit RandomGenerator(const GameParams& params, unsigned int seed = 0);

    bool getRandomBool(int chancePercent);
    int getRandomInRange(int min, int max);

    int getRandomLandPrice();
    bool checkPlague();
};
