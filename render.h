#pragma once //compile once?

#include <stdarg.h> //for debugging
#include <stdio.h>

#include "dungeon.h"
#include "player.h"
#include "helpers.h"

typedef struct {
    Coordinate map_position;
    Coordinate win_midpoint;
    size_t win_h; // play_win width
    size_t win_w; // play_win height
    
    //when to move camera if player is too far
    size_t max_distance_from_player;

    //array of characters(?) that the will be drawn on play_win
    char *camera_view;

} Camera;

void debug_init(const char *path);
void debug_close();

int start_ncurses();
int create_screens();
int end_ncurses();

int initialize_character_placement(Player *p);

void render_map(const Dungeon *d);
Camera initalize_win_camera(Coordinate start_coors);
void update_camera_position(Player *p, Camera *c);
void set_camera_view(Camera *c, D_Floor *f, Player *p);
void render_camera_view(Camera *c);

void reset_debug_row(void);
void print_debug(const char *fmt, ...) __attribute__((format(printf, 1, 2)));