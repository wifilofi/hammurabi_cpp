#include "include/Game.h"
#include <iostream>
#include <Windows.h>
#include "include/InputUtils.h"
#include "include/RoundStorage.h"


void Game::mainLoop()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "\nПриветствую мой повелитель...\n";
    std::cout << "В городе проживает " << game_storage_.cur_people << " человек.\n";
    std::cout << "В закромах города " << game_storage_.cur_wheat << " пшеницы.\n";
    std::cout << "Город занимает " << game_storage_.cur_land << " акров земли.\n";

    RoundStorage prev_round_storage(game_params_, game_storage_, rng_);

    for (; game_storage_.cur_round <= game_params_.max_round; ++game_storage_.cur_round)
    {
        std::cout << "Мой повелитель, соизволь поведать тебе..." << '\n';
        std::cout << "В году " << game_storage_.cur_round << " твоего высочайшего правления" << '\n';


        if (game_storage_.cur_round > 1)
        {
            showRoundReport(prev_round_storage);
        }
        
        RoundStorage round_storage(game_params_, game_storage_, rng_);
        round_storage.generateRound();



        // player input
        int wheat_to_by, wheat_to_sell, wheat_for_food, wheat_to_plant;
        bool valid_input = false;
        while (!valid_input)
        {
            std::cout << "\nЧто пожелаешь, повелитель?" << '\n';
            std::cout << "Сколько акров земли повелеваешь КУПИТЬ?:";
            std::cin >> wheat_to_by;

            std::cout << "Сколько акров земли повелеваешь ПРОДАТЬ?: ";
            std::cin >> wheat_to_sell;

            std::cout << "Сколько бушелей пшеницы повелеваешь СЪЕСТЬ?: ";
            std::cin >> wheat_for_food;

            std::cout << "Сколько акров земли повелеваешь ЗАСЕЯТЬ?: ";
            std::cin >> wheat_to_plant;

            valid_input = round_storage.validatePlayerInput(wheat_to_by, wheat_to_sell,
                                                            wheat_for_food, wheat_to_plant);
        }

        std::cout << "\n========================================\n";


        // calculate round results
        round_storage.calculateRoundResults();
        int alivePeoplePercentage = round_storage.people_died / game_storage_.cur_people;

        //death
        if (alivePeoplePercentage >= game_params_.min_people_percentage)
        {
            std::cout << "Мой повелитель, вы проиграли..." << '\n';
            showFinalStats();
            break;
        }

        //auto-save
        //
        prev_round_storage = round_storage;
    }

    //game over - win
    showFinalStats();

    //remove save
}

void Game::showFinalStats()
{
    std::cout << "\n=== ИТОГИ ВАШЕГО ПРАВЛЕНИЯ ===\n";

    double avg_death_percentage = (game_storage_.total_deaths * 100.0) /
        (game_storage_.cur_round * game_storage_.cur_people);

    double land_per_person = static_cast<double>(game_storage_.cur_land) / game_storage_.cur_people;

    // stats
    std::cout << "Среднегодовой процент умерших от голода: "
        << avg_death_percentage << "%\n";
    std::cout << "Акров земли на одного жителя: " << land_per_person << "\n";

    // very bad ending
    if (avg_death_percentage > 33 && land_per_person < 7)
    {
        std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, "
            << "и изгнал вас из города. Теперь вы вынуждены влачить "
            << "жалкое существование в изгнании.\n";
    }

    //bad ending
    else if (avg_death_percentage > 10 && land_per_person < 9)
    {
        std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. "
            << "Народ вздохнул с облегчением, и никто больше не желает "
            << "видеть вас правителем.\n";
    }

    //neural ending
    else if (avg_death_percentage > 3 && land_per_person < 10)
    {
        std::cout << "Вы справились вполне неплохо, у вас, конечно, есть "
            << "недоброжелатели, но многие хотели бы увидеть вас "
            << "во главе города снова.\n";
    }

    //good ending
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
        std::cout << round_storage.people_died << " человек умерли с голоду, ";
    }
    if (round_storage.people_arrived > 0)
    {
        std::cout << round_storage.people_arrived << " человек прибыли в наш великий город;\n";
    }
    if (round_storage.was_plague)
    {
        std::cout << "Чума уничтожила половину населения;\n";
    }

    std::cout << "Население города сейчас составляет " << game_storage_.cur_people << " человек;\n";
    std::cout << "Мы собрали " << round_storage.wheat_collected << " бушелей пшеницы, по "
        << round_storage.wheat_per_land << " бушеля с акра;\n";

    if (round_storage.wheat_ratted > 0)
    {
        std::cout << "Крысы истребили " << round_storage.wheat_ratted
            << " бушелей пшеницы, ";
    }
    std::cout << "оставив " << game_storage_.cur_wheat << " бушелей в амбарах;\n";
    std::cout << "Город сейчас занимает " << game_storage_.cur_land << " акров;\n";
    std::cout << "1 акр земли стоит сейчас " << round_storage.land_price << " бушель.\n";
    std::cout << "========================================\n";
}
