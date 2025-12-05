#include "include/Game.h"
#include <iostream>
#include <Windows.h>

#include "include/RoundStorage.h"


void Game::mainLoop()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    for (; game_storage_.cur_round <= game_params_.max_round; ++game_storage_.cur_round)
    {
        std::cout << "Мой повелитель, соизволь поведать тебе..." << '\n';
        std::cout << "В году " << game_storage_.cur_round << " твоего высочайшего правления" << '\n';
        
        RoundStorage round_storage(game_params_, game_storage_, rng_);
        round_storage.processRound();

        if (game_storage_.cur_round > 1)
        {
            showRoundReport(round_storage, game_storage_);
        }

        // player input
        int wheat_to_by, wheat_to_sell, wheat_for_food, wheat_to_plant;
        bool valid_input = false;
        while (!valid_input)
        {
            std::cout << "\nЧто пожелаешь, повелитель? \n";
            std::cout << "Сколько акров земли повелеваешь купить? ";
            std::cin >> wheat_to_by;

            std::cout << "Сколько акров земли повелеваешь продать? ";
            std::cin >> wheat_to_sell;

            std::cout << "Сколько бушелей пшеницы повелеваешь съесть? ";
            std::cin >> wheat_for_food;

            std::cout << "Сколько акров земли повелеваешь засеять? ";
            std::cin >> wheat_to_plant;

            valid_input = round_storage.validatePlayerInput(wheat_to_by, wheat_to_sell,
                                                            wheat_for_food, wheat_to_plant);
        }

        // calculate round results
        round_storage.calculateRoundResults();

        int alivePeoplePercentage = round_storage.people_died / game_storage_.cur_people;

        //death
        if (alivePeoplePercentage >= game_params_.min_people_percentage)
        {
            std::cout << "Мой повелитель, вы проиграли..." << '\n';
            showFinalStats();
        }

        //auto-save

        //

        game_storage_.cur_people -= round_storage.people_died;

        //cap to 0
        game_storage_.cur_wheat -= game_storage_.getNeededWheat();
    }

    //game over - win
    showFinalStats();

    //remove save
}

void Game::showFinalStats()
{
   
}

void Game::showRoundReport(const RoundStorage& round_storage, const GameStorage& gameStorage)
{
}
