#pragma once
#include "GameStorage.h"
#include "RandomGenerator.h"

struct RoundStorage
{
    explicit RoundStorage(const GameParams& params, const GameStorage& game_storage, const RandomGenerator& rng);
public:
    int people_died;
    int people_arrived;

    int wheat_collected;
    int wheat_ratted;
    bool was_plague = false;
    
    //this round
    int land_price;
    int wheat_after_trade = 0;
    int wheat_per_land = 0;
    int land_after_trade = 0;

    void generateRound();
    bool validatePlayerInput(int wheat_to_buy, int wheat_to_sell, int wheat_for_food, int wheat_to_plant);
    void calculateRoundResults();

private:
    GameParams game_params_;
    GameStorage game_storage_;
    RandomGenerator rng_;
};
