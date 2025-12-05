#include <iostream>
#include <Windows.h>

#include "include/GameStorage.h"
#include "include/GameParams.h"
#include "include/RandomGenerator.h"
#include "include/RoundStorage.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    GameParams game_params;
    GameStorage game_storage(game_params);
    RandomGenerator rng(game_params, std::random_device{}());

    for (; game_storage.cur_round <= game_params.max_round; ++game_storage.cur_round)
    {
        std::cout << "Мой повелитель, соизволь поведать тебе..." << '\n';
        std::cout << "В году " << game_storage.cur_round << " твоего высочайшего правления" << '\n';

        RoundStorage round_storage(game_params, game_storage, rng);
        round_storage.processRound();

        int alivePeoplePercentage = round_storage.people_died / game_storage.cur_people;

        if (alivePeoplePercentage >= game_params.min_people_percentage)
        {
            std::cout << "Мой повелитель, вы проиграли..." << '\n';
            return 0;
            //death
        }

        game_storage.cur_people -= round_storage.people_died;

        //cap to 0
        game_storage.cur_wheat -= game_storage.cur_people * game_params.wheat_per_person;
    }

    //game over win
}
