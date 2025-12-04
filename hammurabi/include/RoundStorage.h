#pragma once
#include "GameStorage.h"
#include "RandomGenerator.h"


struct RoundStorage
{
    RoundStorage(const GameParams& params, const GameStorage& game_storage, const RandomGenerator& rng)
        : params_(params),
          game_storage_(game_storage),
          rng_(rng)
    {
    }

private:
    GameParams params_;
    GameStorage game_storage_;
    RandomGenerator rng_;

public:
    int people_died;
    int people_arrived;

    int wheat_collected;
    int wheat_ratted;

    int landPrice;

    bool wasPlague = false;
};
