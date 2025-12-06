#include "include/Game.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include "include/InputUtils.h"
#include "include/RoundStorage.h"

void Game::mainLoop()
{
    while (game_storage_.cur_round <= game_params_.max_round)
    {
        std::cout << "\n========================================\n";
        std::cout << "Мой повелитель, соизволь поведать тебе..." << '\n';
        std::cout << "В году " << game_storage_.cur_round << " твоего высочайшего правления" << '\n';

        if (game_storage_.cur_round > 1)
        {
            showRoundReport(prev_round_storage_);
        }

        std::cout << "\nВ городе проживает " << game_storage_.cur_people << " человек.\n";
        std::cout << "В закромах города " << game_storage_.cur_wheat << " пшеницы.\n";
        std::cout << "Город занимает " << game_storage_.cur_land << " акров земли.\n\n";

        RoundStorage round_storage(game_params_, game_storage_, rng_);
        round_storage.generateRound();

        // player input
        int wheat_to_buy, wheat_to_sell, wheat_for_food, wheat_to_plant;
        bool valid_input = false;
        while (!valid_input)
        {
            std::cout << "Что пожелаешь, повелитель?" << '\n';
            std::cout << "Сколько акров земли повелеваешь КУПИТЬ?: ";
            std::cin >> wheat_to_buy;

            std::cout << "Сколько акров земли повелеваешь ПРОДАТЬ?: ";
            std::cin >> wheat_to_sell;

            std::cout << "Сколько бушелей пшеницы повелеваешь СЪЕСТЬ?: ";
            std::cin >> wheat_for_food;

            std::cout << "Сколько акров земли повелеваешь ЗАСЕЯТЬ?: ";
            std::cin >> wheat_to_plant;

            valid_input = round_storage.validatePlayerInput(wheat_to_buy, wheat_to_sell,
                                                            wheat_for_food, wheat_to_plant);
        }

        // calculate round results
        round_storage.calculateRoundResults();

        // check lose condition
        int death_percentage = (round_storage.people_died * 100) / game_storage_.cur_people;
        if (death_percentage >= game_params_.min_people_percentage)
        {
            std::cout << "\n========================================\n";
            std::cout << "Мой повелитель, вы нас погубили...";
            std::cout << "\n========================================\n";
            game_storage_.avg_death_percentage = death_percentage;

            showFinalStats();

            std::remove("save.dat");
            break;
        }

        // update game state
        game_storage_.cur_people = game_storage_.cur_people - round_storage.people_died + round_storage.people_arrived;
        if (round_storage.was_plague)
        {
            game_storage_.cur_people /= 2;
        }

        game_storage_.cur_wheat = round_storage.wheat_after_trade;
        game_storage_.cur_land = round_storage.land_after_trade;
        game_storage_.total_deaths += round_storage.people_died;
        game_storage_.total_arrivals += round_storage.people_arrived;

        //TODO: refactor
        prev_round_storage_.people_died = round_storage.people_died;
        prev_round_storage_.people_arrived = round_storage.people_arrived;
        prev_round_storage_.wheat_collected = round_storage.wheat_collected;
        prev_round_storage_.wheat_ratted = round_storage.wheat_ratted;
        prev_round_storage_.was_plague = round_storage.was_plague;
        prev_round_storage_.land_price = round_storage.land_price;
        prev_round_storage_.wheat_after_trade = round_storage.wheat_after_trade;
        prev_round_storage_.wheat_per_land = round_storage.wheat_per_land;
        prev_round_storage_.land_after_trade = round_storage.land_after_trade;

        // ask for save/quit
        std::cout << "\n1. Продолжить игру\n";
        std::cout << "2. Сохранить и выйти\n";
        std::cout << "Выберите действие: ";
        int choice;
        std::cin >> choice;

        if (choice == 2)
        {
            // save game
            std::ofstream saveFile("save.dat", std::ios::binary);
            saveFile.write(reinterpret_cast<const char*>(&game_storage_.cur_people), sizeof(int));
            saveFile.write(reinterpret_cast<const char*>(&game_storage_.cur_wheat), sizeof(int));
            saveFile.write(reinterpret_cast<const char*>(&game_storage_.cur_land), sizeof(int));
            saveFile.write(reinterpret_cast<const char*>(&game_storage_.cur_round), sizeof(int));
            saveFile.write(reinterpret_cast<const char*>(&game_storage_.needed_wheat), sizeof(int));
            saveFile.write(reinterpret_cast<const char*>(&game_storage_.total_deaths), sizeof(int));
            saveFile.write(reinterpret_cast<const char*>(&game_storage_.total_arrivals), sizeof(int));
            saveFile.close();
            return;
        }

        game_storage_.cur_round++;
    }

    if (game_storage_.cur_round > game_params_.max_round)
    {
        std::cout << "\nПоздравляю! Вы успешно правили 10 лет!\n";
        showFinalStats();
    }

    std::remove("save.dat");
}

void Game::showFinalStats()
{
    std::cout << "\n=== ИТОГИ ВАШЕГО ПРАВЛЕНИЯ ===\n";

    double avg_death_percentage = 0;
    if (game_storage_.cur_round > 1)
    {
        avg_death_percentage = (game_storage_.total_deaths * 100.0) /
        (game_storage_.cur_round * (game_storage_.cur_people + game_storage_.total_deaths - game_storage_.
            total_arrivals));
    }
    else
    {
        avg_death_percentage = game_storage_.avg_death_percentage;
    }

    double land_per_person = static_cast<double>(game_storage_.cur_land) / game_storage_.cur_people;

    std::cout << "Среднегодовой процент умерших от голода: "
        << avg_death_percentage << "%\n";
    std::cout << "Акров земли на одного жителя: " << land_per_person << "\n";

    if (avg_death_percentage >= game_params_.min_people_percentage)
    {
        return;
    }
    if (avg_death_percentage > 33 && land_per_person < 7)
    {
        std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, "
            << "и изгнал вас из города. Теперь вы вынуждены влачить "
            << "жалкое существование в изгнании.\n";
    }
    else if (avg_death_percentage > 10 && land_per_person < 9)
    {
        std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. "
            << "Народ вздохнул с облегчением, и никто больше не желает "
            << "видеть вас правителем.\n";
    }
    else if (avg_death_percentage > 3 && land_per_person < 10)
    {
        std::cout << "Вы справились вполне неплохо, у вас, конечно, есть "
            << "недоброжелатели, но многие хотели бы увидеть вас "
            << "во главе города снова.\n";
    }
    else
    {
        std::cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон "
            << "вместе не справились бы лучше!\n";
    }
}

void Game::showRoundReport(const RoundStorage& round_storage)
{
    if (round_storage.people_died > 0)
    {
        std::cout << round_storage.people_died << " человек умерли с голоду";
        if (round_storage.people_arrived > 0)
        {
            std::cout << ", ";
        }
        else
        {
            std::cout << ";\n";
        }
    }

    if (round_storage.people_arrived > 0)
    {
        std::cout << round_storage.people_arrived << " человек прибыли в наш великий город;\n";
    }

    if (round_storage.was_plague)
    {
        std::cout << "Чума уничтожила половину населения;\n";
    }

    std::cout << "Мы собрали " << round_storage.wheat_collected << " бушелей пшеницы, по "
        << round_storage.wheat_per_land << " бушеля с акра;\n";

    if (round_storage.wheat_ratted > 0)
    {
        std::cout << "Крысы истребили " << round_storage.wheat_ratted
            << " бушелей пшеницы, ";
    }
    std::cout << "оставив " << round_storage.wheat_after_trade << " бушелей в амбарах;\n";
    std::cout << "1 акр земли стоит сейчас " << round_storage.land_price << " бушель.\n";
}
