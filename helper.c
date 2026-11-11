#include "helpers.h"

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void set_seed(uint32_t seed) {
    srand(seed);
};

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
Coordinate coordinate_from_flat_map(uint32_t i, uint16_t w, uint16_t h) {
    /*
        Pos i in array[100] which represents a 10w x 10h map
        i = 67 -> Coor (6y, 7x)
    */

    uint16_t row = (uint16_t) i / w; // Floor Operation of division
    uint16_t col = i - (row * w); // Techincally just the modulo operator?

    Coordinate new_coor = {
        .y = row,
        .x = col
    };

    return new_coor;
}

uint32_t flat_poistion_from_coordinate(Coordinate coor, uint16_t w, uint16_t h) {
    uint32_t flat_position = (coor.y * w) + coor.x;
    return flat_position;
}

uint32_t get_random_int(uint32_t min_val, uint32_t max_val) {
    if (max_val <= min_val) return min_val;   // guards against underflow / div by zero

    uint32_t range = (uint32_t) max_val - min_val + 1;
    return (uint32_t)(min_val + rand() % range);
}