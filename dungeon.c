#include "dungeon.h"
#include <stdbool.h>

// static const D_Terrain Terrain_Table[T_COUNT] = {
//     [T_FLOOR] = { ' ', false},
//     [T_WALL] = { '#', true}
// };

D_Tile make_tile_from_terrain_type(Terrain_Type terrain_type) {
    D_Tile tile = {0};

    if (terrain_type == T_OUTSIDE) {
        tile.terrain_type = T_OUTSIDE;
        tile.is_blocker = true;
        tile.sprite = ' ';
        tile.is_visible = true;
    } else if (terrain_type == T_WALL) {
        tile.terrain_type = T_WALL;
        tile.is_blocker = true;
        tile.sprite = '#';
        tile.is_visible = true;
    } else if (terrain_type == T_FLOOR) {
        tile.terrain_type = T_FLOOR;
        tile.is_blocker = false;
        tile.sprite = '.';
        tile.is_visible = true;
    }

    return tile;
}