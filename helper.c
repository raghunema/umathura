#include "helpers.h"
#include "ncurses.h"

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