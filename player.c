#include "player.h"

Player intialize_player(Coordinates start_coors) {
    Player p = {
        .coors = start_coors,
        .sprite = '@'
    };

    return p;
}