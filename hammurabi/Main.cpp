#include <iostream>
#include <Windows.h>

#include "include/GameStorage.h"
#include "include/GameParams.h"
#include "include/RandomGenerator.h"
#include "include/RoundStorage.h"

int main()
{
    GameStorage game_storage;
    GameParams game_params;
    RandomGenerator rng(game_params, std::random_device{}());

    
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    for (; game_storage.cur_round <= game_params.max_round; ++game_storage.cur_round)
    {
        std::cout << "Мой повелитель, соизволь поведать тебе..." << '\n';
        std::cout << "В году " << game_storage.cur_round << " твоего высочайшего правления" << '\n';

        RoundStorage round_storage(game_params, game_storage, rng);

        if (game_storage.cur_round > 1)
        {
        }
    }

    //game over win
}
