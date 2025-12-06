#pragma once
#include "GameParams.h"


struct GameStorage
{
public:
    //TODO: init via params
    int cur_people = 100;
    int cur_wheat = 2800;
    int cur_land = 1000;

    int cur_round = 1;
    int needed_wheat = 0;
    int total_deaths;
    int total_arrivals;
    int avg_death_percentage;

    GameStorage(GameParams& game_params) :
        game_params_(game_params)
    {
    }

    GameStorage(int cur_people, int cur_wheat, int cur_land, int round, int needed_wheat)
        : cur_people(cur_people),
          cur_wheat(cur_wheat),
          cur_land(cur_land),
          cur_round(round),
          needed_wheat(needed_wheat)
    {
    }

    int getNeededWheat();

private:
    GameParams game_params_;
};
