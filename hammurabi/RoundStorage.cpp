// RoundStorage.cpp
#include "include/RoundStorage.h"
#include <iostream>
#include <Windows.h>
#include "include/MathUtils.h"

RoundStorage::RoundStorage(const GameParams& params,
                           GameStorage& game_storage,
                           const RandomGenerator& rng) :
    rng_(rng),
    game_params_(params),
    game_storage_(game_storage)
{
}

void RoundStorage::generateRound()
{
    land_price = rng_.getRandomLandPrice();
}

bool RoundStorage::validatePlayerInput(int wheat_to_buy, int wheat_to_sell,
                                       int wheat_for_food, int wheat_to_plant)
{
    if (wheat_to_buy < 0 || wheat_to_sell < 0 ||
        wheat_for_food < 0 || wheat_to_plant < 0)
    {
        std::cout << "О, повелитель, числа не могут быть отрицательными!\n";
        return false;
    }

    if (wheat_to_sell > game_storage_.cur_land)
    {
        std::cout << "У нас нет столько земли для продажи!\n";
        return false;
    }

    land_after_trade = game_storage_.cur_land + wheat_to_buy - wheat_to_sell;
    wheat_after_trade = game_storage_.cur_wheat
        - wheat_to_buy * land_price
        + wheat_to_sell * land_price;

    if (wheat_after_trade < 0)
    {
        std::cout << "У нас недостаточно пшеницы для покупки земли!\n";
        return false;
    }

    if (wheat_to_plant > land_after_trade)
    {
        std::cout << "У нас нет столько земли для посева!\n";
        return false;
    }

    int seeds_needed = (int)((float)wheat_to_plant * game_params_.wheat_per_land);
    if (seeds_needed > wheat_after_trade)
    {
        std::cout << "У нас недостаточно пшеницы для семян!\n";
        return false;
    }

    if (wheat_for_food > wheat_after_trade - seeds_needed)
    {
        std::cout << "У нас недостаточно пшеницы для еды!\n";
        return false;
    }

    int max_acres_can_plant = game_storage_.cur_people * game_params_.person_per_land_plant;
    if (wheat_to_plant > max_acres_can_plant)
    {
        std::cout << "У нас недостаточно людей для обработки всей земли!\n";
        return false;
    }

    return true;
}

void RoundStorage::calculateRoundResults()
{
    // apply trade
    game_storage_.cur_land = land_after_trade;
    game_storage_.cur_wheat = wheat_after_trade;

    // calculate deaths from starvation
    int wheat_needed = game_storage_.cur_people * game_params_.wheat_per_person;
    int wheat_allocated = (wheat_for_food < wheat_needed) ? wheat_for_food : wheat_needed;
    int people_fed = wheat_allocated / game_params_.wheat_per_person;
    people_died = (game_storage_.cur_people > people_fed) ? game_storage_.cur_people - people_fed : 0;

    // subtract food and seeds
    int seeds_used = (int)((float)wheat_to_plant * game_params_.wheat_per_land);
    game_storage_.cur_wheat = game_storage_.cur_wheat - wheat_allocated - seeds_used;

    // calculate harvest
    wheat_per_land = rng_.getRandomInRange(game_params_.min_wheat_per_land, game_params_.max_wheat_per_land);
    wheat_collected = wheat_to_plant * wheat_per_land;
    game_storage_.cur_wheat += wheat_collected;

    // rats
    double rat_percentage = rng_.getRandomInRange(
        game_params_.min_ratted_percentage, game_params_.max_rated_percentage) / 100.0;
    wheat_ratted = static_cast<int>(game_storage_.cur_wheat * rat_percentage);
    game_storage_.cur_wheat -= wheat_ratted;

    // calculate arrivals
    int base_arrivals = people_died / 2;
    int wheat_factor = (5 - wheat_per_land) * game_storage_.cur_wheat / 600 + 1;
    int temp_arrivals = base_arrivals + wheat_factor;

    temp_arrivals = MathUtils::clamp(temp_arrivals, game_params_.min_arrived, game_params_.max_arrived);
    people_arrived = temp_arrivals;

    // plague
    was_plague = rng_.checkPlague();

    // final wheat after round
    wheat_after_trade = game_storage_.cur_wheat;
}
