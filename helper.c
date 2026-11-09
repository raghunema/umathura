#include "helpers.h"

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

Coordinate get_new_coors_from_input(Coordinate prev_coors, int ch) {
    Coordinate new_coors = {
        .x = prev_coors.x,
        .y = prev_coors.y
    };

    switch (ch) {
        case KEY_DOWN: 
            new_coors.y++;
            break;
        case KEY_UP:
            new_coors.y--;
            break;
        case KEY_RIGHT:
            new_coors.x++;
            break;
        case KEY_LEFT:
            new_coors.x--;
            break;
        default:
            break;
    }

    return new_coors;
}


/*
    Given a position from a flat_map, out put a coordinate
*/
Coordinate coordinate_from_flat_map(size_t i, size_t w, size_t h) {
    /*
        Pos i in array[100] which represents a 10w x 10h map
        i = 67 -> Coor (6y, 7x)
    */

    size_t row = (size_t) i / w; // Floor Operation of division
    size_t col = i - (row * w); // Techincally just the modulo operator?

    Coordinate new_coor = {
        .y = row,
        .x = col
    };

    return new_coor;
}

size_t flat_poistion_from_coordinate(Coordinate coor, size_t w, size_t h) {
    size_t flat_position = (coor.y * w) + coor.x;
    return flat_position;
}

uint16_t get_random_int(uint16_t min_val, uint16_t max_val, uint16_t seed_shift) {
    srand(time(NULL) + (seed_shift * seed_shift)); //set seed to NOW;

    int16_t rand_int = (rand() % (max_val - min_val + 1)) + min_val;
    return rand_int;
}