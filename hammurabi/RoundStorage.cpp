#include "include/RoundStorage.h"

RoundStorage::RoundStorage(const GameParams& params,
                           const GameStorage& game_storage,
                           const RandomGenerator& rng) :
    rng_(rng),
    game_params_(params),
    game_storage_(game_storage)
{
}

void RoundStorage::processRound()
{
    land_price = rng_.getRandomLandPrice();
    
    int maxFedPeople = game_storage_.cur_wheat / game_params_.wheat_per_person;
    people_died = (maxFedPeople < game_storage_.cur_people) ? game_storage_.cur_people - maxFedPeople : 0;
}

// Based on generated round
void RoundStorage::updateGameStorage()
{
    
}
