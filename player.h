#pragma once //compile once?

#include "helpers.h"
#include "stdbool.h"

typedef struct {
    Coordinate coors;
    char sprite;
    uint16_t xp;
    uint8_t level;
    uint8_t movement; //how many moves per loop?
    uint16_t ac; //armor class
    int16_t max_hp; //ig debuffs could technically make max_hp negative
    int16_t hp; //need to be negative to take damage overflow into account?
    Item weapon;
    Item *armor;
} Player;

Player intialize_player(Coordinate start_coors);

int initialize_character_placement(Player *p);
void update_character_position(Player *p, Coordinate new_coors);