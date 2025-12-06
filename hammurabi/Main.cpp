#include <iostream>
#include <Windows.h>
#include <fstream>

#include "include/Game.h"
#include "include/GameStorage.h"
#include "include/GameParams.h"
#include "include/RandomGenerator.h"

bool fileExists(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    bool loadSaved = false;

    if (fileExists("save.dat"))
    {
        std::cout << "Обнаружено сохранение. Продолжить предыдущую игру? (1-да, 0-нет): ";
        int choice;
        std::cin >> choice;
        loadSaved = (choice == 1);
    }

    GameParams game_params;
    GameStorage game_storage(game_params);
    RandomGenerator rng(game_params, std::random_device{}());

    if (loadSaved)
    {
        std::ifstream saveFile("save.dat", std::ios::binary);
        if (saveFile)
        {
            saveFile.read(reinterpret_cast<char*>(&game_storage.cur_people), sizeof(int));
            saveFile.read(reinterpret_cast<char*>(&game_storage.cur_wheat), sizeof(int));
            saveFile.read(reinterpret_cast<char*>(&game_storage.cur_land), sizeof(int));
            saveFile.read(reinterpret_cast<char*>(&game_storage.cur_round), sizeof(int));
            saveFile.read(reinterpret_cast<char*>(&game_storage.needed_wheat), sizeof(int));
            saveFile.read(reinterpret_cast<char*>(&game_storage.total_deaths), sizeof(int));
            saveFile.read(reinterpret_cast<char*>(&game_storage.total_arrivals), sizeof(int));
            saveFile.close();
        }
    }

    Game game(game_storage, game_params, rng);
    game.mainLoop();
}
