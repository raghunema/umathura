#pragma once //compile once?

#include "helpers.h"
#include "stdbool.h"
#include "ncurses.h"

typedef struct {
    Coordinate coors;
    char sprite;
} Player;

Player intialize_player(Coordinate start_coors);

int initialize_character_placement(Player *p);
bool is_character_input(Player *p, int ch);