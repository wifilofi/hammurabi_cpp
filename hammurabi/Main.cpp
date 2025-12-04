#include <iostream>
#include <Windows.h>

#include "Game.h"

int main()
{
    struct GameStorage game_storage;
    struct GameParams game_params;

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    for (; game_storage.cur_round <= game_params.max_round; ++ game_storage.cur_round)
    {
        std::cout << "Мой повелитель, соизволь поведать тебе..." << '\n';
        std::cout << "В году " << game_storage.cur_round << " твоего высочайшего правления" << '\n';
    }
}