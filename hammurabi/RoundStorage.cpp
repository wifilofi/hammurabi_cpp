#include "include/RoundStorage.h"
#include <iostream>
#include <Windows.h>
#include "include/MathUtils.h"

RoundStorage::RoundStorage(const GameParams& params,
                           const GameStorage& game_storage,
                           const RandomGenerator& rng) :
    rng_(rng),
    game_params_(params),
    game_storage_(game_storage)
{
    //TODO: refactor to sep class with print
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void RoundStorage::processRound()
{
    land_price = rng_.getRandomLandPrice();

    int maxFedPeople = game_storage_.cur_wheat / game_params_.wheat_per_person;
    people_died = (maxFedPeople < game_storage_.cur_people) ? game_storage_.cur_people - maxFedPeople : 0;
}

bool RoundStorage::validatePlayerInput(int wheat_to_buy, int wheat_to_sell,
                                       int wheat_for_food, int wheat_to_plant)
{
    // negative
    if (wheat_to_buy < 0 || wheat_to_sell < 0 ||
        wheat_for_food < 0 || wheat_to_plant < 0)
    {
        std::cout << "О, повелитель, числа не могут быть отрицательными!\n";
        return false;
    }

    // enough land to sell
    if (wheat_to_sell > game_storage_.cur_land)
    {
        std::cout << "У нас нет столько земли для продажи!\n";
        return false;
    }

    // land trade
    land_after_trade = game_storage_.cur_land + wheat_to_buy - wheat_to_sell;
    wheat_after_trade = game_storage_.cur_wheat
        - wheat_to_buy * land_price
        + wheat_to_sell * land_price;

    // enough wheat to buy land
    if (wheat_after_trade < 0)
    {
        std::cout << "У нас недостаточно пшеницы для покупки земли!\n";
        return false;
    }

    // enough land to plant
    if (wheat_to_plant > land_after_trade)
    {
        std::cout << "У нас нет столько земли для посева!\n";
        return false;
    }

    // enough wheat to plant
    // TODO: remove weird cast
    int seeds_needed = (int)((float)wheat_to_plant * game_params_.wheat_per_land);
    if (seeds_needed > wheat_after_trade)
    {
        std::cout << "У нас недостаточно пшеницы для семян!\n";
        return false;
    }

    // enough wheat to eat
    if (wheat_for_food > wheat_after_trade - seeds_needed)
    {
        std::cout << "У нас недостаточно пшеницы для еды!\n";
        return false;
    }

    // enough people for plant
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
    int wheat_for_food;
    if (game_storage_.cur_wheat < game_storage_.cur_people * game_params_.wheat_per_person)
        wheat_for_food = game_storage_.cur_wheat;
    else
        wheat_for_food = game_storage_.cur_people * game_params_.wheat_per_person;

    int people_can_feed = wheat_for_food / game_params_.wheat_per_person;
    people_died = (game_storage_.cur_people > people_can_feed) ? game_storage_.cur_people - people_can_feed : 0;

    // update people
    game_storage_.cur_people -= people_died;
    game_storage_.total_deaths += people_died;

    // wheat per land
    wheat_per_land = rng_.getRandomInRange(game_params_.min_wheat_per_land, game_params_.max_wheat_per_land);
    wheat_collected = game_storage_.cur_land * wheat_per_land;

    // rat 
    double rat_percentage = rng_.getRandomInRange(
        game_params_.min_ratted_percentage, game_params_.max_rated_percentage) / 100.0;
    wheat_ratted = static_cast<int>(game_storage_.cur_wheat * rat_percentage);

    // arrived people
    int base_arrivals = people_died / 2;
    int wheat_factor = (5 - wheat_per_land) * game_storage_.cur_wheat / 600;
    int temp_arrivals = base_arrivals + wheat_factor + 1;

    MathUtils::clamp(temp_arrivals, game_params_.min_arrived, game_params_.max_arrived);

    people_arrived = temp_arrivals;

    game_storage_.cur_people += people_arrived;
    game_storage_.total_arrivals += people_arrived;

    // plague
    if (rng_.checkPlague())
    {
        was_plague = true;
        game_storage_.cur_people /= 2;
    }

    // wheat update
    game_storage_.cur_wheat = game_storage_.cur_wheat
        - wheat_for_food
        + wheat_collected
        - wheat_ratted;
}
