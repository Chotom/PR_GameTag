#pragma once
#include "consts.h"

struct InMessage {
    //int game_state; // 0 - prepare game; 1 - game in progress; 2 - end of game
    int pos_x[CLIENTS_COUNT];
    int pos_y[CLIENTS_COUNT];
};
