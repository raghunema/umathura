#pragma once //compile once?

#include "dungeon.h"
#include "player.h"

int start_ncurses();
int create_screens();
int end_ncurses();

int initialize_character_placement(Player *p);
void render_map(const Dungeon *d);