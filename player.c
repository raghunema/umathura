#include "player.h"
#include "ncurses.h"
#include "render.h"

Player intialize_player(Coordinate start_coors) {
    Player p = {
        .coors = start_coors,
        .sprite = '@'
    };

    return p;
};

bool is_character_input(Player *p, int ch) {
    //print_debug("character_move: %c", ch);
    print_debug("ch = %d", ch);
    print_debug("KEY_DOWN=%d KEY_UP=%d KEY_LEFT=%d KEY_RIGHT=%d", KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT);

    switch (ch) {
        case KEY_DOWN: 
            p->coors.y++;
            print_debug("new character postion (x, y): (%zu, %zu)", p->coors.x, p->coors.y);
            return true;
        case KEY_UP:
            p->coors.y--;
            print_debug("new character postion (x, y): (%zu, %zu)", p->coors.x, p->coors.y);
            return true;
        case KEY_RIGHT:
            p->coors.x++;
            print_debug("new character postion (x, y): (%zu, %zu)", p->coors.x, p->coors.y);
            return true;
        case KEY_LEFT:
            p->coors.x--;
            print_debug("new character postion (x, y): (%zu, %zu)", p->coors.x, p->coors.y);
            return true;
        default:
            return false;
    }

}