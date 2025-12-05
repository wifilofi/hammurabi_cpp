#include <iostream>
#include <Windows.h>

#include "include/Game.h"
#include "include/GameStorage.h"
#include "include/GameParams.h"
#include "include/RandomGenerator.h"
#include "include/RoundStorage.h"


int main()
{
    
    GameParams game_params;
    GameStorage game_storage(game_params);
    RandomGenerator rng(game_params, std::random_device{}());
    Game game(game_storage, game_params, rng);

    game.mainLoop();
    //game over win
}
