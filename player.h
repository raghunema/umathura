#pragma once //compile once?

#include "helpers.h"
#include "stdbool.h"

typedef struct {
    Coordinate coors;
    char sprite;
} Player;

Player intialize_player(Coordinate start_coors);

int initialize_character_placement(Player *p);
void update_character_position(Player *p, Coordinate new_coors);