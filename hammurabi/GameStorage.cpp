#include "include/GameStorage.h"

int GameStorage::getNeededWheat()
{
    return cur_people * game_params_.wheat_per_person;
}