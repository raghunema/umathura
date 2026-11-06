#include "player.h"

Player intialize_player(Coordinate start_coors) {
    Player p = {
        .coors = start_coors,
        .sprite = '@'
    };

    return p;
};

bool is_character_input(Player *p, int ch) {
    
    switch (ch) {
        case KEY_DOWN: 
            p->coors.y++;
            return true;
        case KEY_UP:
            p->coors.y--;
            return true;
        case KEY_RIGHT:
            p->coors.x++;
            return true;
        case KEY_LEFT:
            p->coors.x--;
            return true;
        default:
            return false;
    }

}