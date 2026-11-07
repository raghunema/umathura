#include "player.h"
#include "render.h"

Player intialize_player(Coordinate start_coors) {
    Player p = {
        .coors = start_coors,
        .sprite = '@'
    };

    return p;
};

void update_character_position(Player *p, Coordinate new_coors) {
    p->coors = new_coors;
}