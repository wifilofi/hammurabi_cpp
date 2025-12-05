#pragma once

struct GameParams
{
public:
    int max_round = 10;
    int plague_chance = 15;
    int wheat_per_person = 20;
    int min_people_percentage = 45;
    float wheat_per_land = 0.5;
    int person_per_land_plant = 10;

    int min_wheat_per_land = 1;
    int max_wheat_per_land = 6;

    int min_arrived = 0;
    int max_arrived = 50;

    int min_ratted_percentage = 0;
    int max_rated_percentage = 7;
    
    int min_land_price = 17;
    int max_land_price = 26;
};