#pragma once
#include "GameStorage.h"
#include "RandomGenerator.h"

struct RoundStorage
{
    explicit RoundStorage(const GameParams& params, const GameStorage& game_storage, const RandomGenerator& rng);

private:
    GameParams game_params_;
    GameStorage game_storage_;
    RandomGenerator rng_;


public:
    int people_died;
    int people_arrived;

    int wheat_collected;
    int wheat_ratted;

    int land_price;

    bool was_plague = false;

    void processRound();
    void updateGameStorage();
};
