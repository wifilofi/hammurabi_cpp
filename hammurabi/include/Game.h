#pragma once
#include "GameParams.h"
#include "GameStorage.h"
#include "RandomGenerator.h"
#include "RoundStorage.h"

class Game
{
public:
    void mainLoop();
    void showFinalStats();

    Game(const GameStorage& game_storage, const GameParams& game_params, const RandomGenerator& rng)
        : game_storage_(game_storage),
          game_params_(game_params),
          rng_(rng)
    {
    }

private:
    GameStorage game_storage_;
    GameParams game_params_;
    RandomGenerator rng_;

    void showRoundReport(const RoundStorage& round_storage, const GameStorage& gameStorage);
    
};
