#include "dungeon.h"

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

D_Floor floor_from_text(const char *char_map, uint32_t h, uint32_t w) {
    D_Floor f;
    f.floor_height = h;
    f.floor_width = w;
    f.tiles = malloc((size_t)w * h * sizeof(D_Tile));

    for (size_t i = 0; i < (size_t)w * h; i++) {
        D_Tile tile;
        if (char_map[i] == '#') {
            tile = make_tile_from_terrain_type(T_WALL);
        } else if (char_map[i] == '.') {
            tile = make_tile_from_terrain_type(T_FLOOR);            
        } else {
            tile = make_tile_from_terrain_type(T_OUTSIDE);            
        } 

        f.tiles[i] = tile;
    }

    return f;
}

