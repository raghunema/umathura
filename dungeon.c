#include "dungeon.h"
#include "helpers.h"
#include "render.h"

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
    } else if (terrain_type == T_UNSET) {
        tile.terrain_type = T_OUTSIDE;
        tile.is_blocker = true;
        tile.sprite = ' ';
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

typedef struct {
    uint16_t x1;
    uint16_t x2;
    uint16_t w; 
    uint16_t y1;
    uint16_t y2;
    uint16_t h;
} D_Room;

D_Floor generate_floor(uint16_t h, uint16_t w, uint16_t room_tries) {
    D_Floor f = {
        .floor_height = h,
        .floor_width = w
    };

    //Allocate memory for a tile array
    f.tiles = malloc((uint16_t) w * h * sizeof (D_Tile));

    /*
        For 0 to room_tries
        Spawn a room with at random coordinates (a, b) with variable (h, w)
        Check if that room is inbound and doesn't intersect with another room 
        If ok, create tiles and save to floor
    */


    for (uint16_t i = 0; i < (h * w); i++) {
        f.tiles[i] = make_tile_from_terrain_type(T_OUTSIDE);
    }

    //Max possible rooms is if all rooms are valid
    D_Room *rooms = malloc(room_tries * sizeof(D_Room));

    for (uint16_t i = 0; i <= room_tries; i++) {
        D_Room temp_room = {
            .w = get_random_int(3, (uint16_t) w / 8, i), // picking 8 randomly,
            .h = get_random_int(3, (uint16_t) h / 8, i), // picking 8 randomly
        };

        uint16_t room_w  = get_random_int(3, (uint16_t) w / 8, i); // picking 8 randomly
        uint16_t room_h = get_random_int(3, (uint16_t) h / 8, i); // picking 8 randomly
        
        //Top left corner of the box - needs to shift to account for boundaries
        uint16_t room_x = get_random_int(0, w - (room_w + 1), i);
        uint16_t room_y = get_random_int(0, h - (room_h + 1), i);

        /*
            Add collision check here
        */

        print_debug("\n(y,x): %hu, %hu; (h,w): %hu, %hu", room_y, room_x, room_h, room_w);

        for (uint16_t j = room_y; j < (room_y + room_h); j++) {
            for (uint16_t k = room_x; k < (room_x + room_w); k++) {
                uint16_t r_index = (j * w) + k;
                //print_debug("r_index: %hu\n", r_index);
                //f.tiles[r_index] = make_tile_from_terrain_type(T_FLOOR);

                //wall placement
                if (
                    j == room_y || 
                    j == (room_y + room_h - 1) ||
                    k == room_x || 
                    k == (room_x + room_w - 1)
                ) {
                    f.tiles[r_index] = make_tile_from_terrain_type(T_WALL);
                } else {
                    f.tiles[r_index] = make_tile_from_terrain_type(T_FLOOR);
                }
            }
        }
    }

    // for (uint16_t i = 0; i < (h * w); i++) {
    //     D_Tile temp_tile = f.tiles[i];
    //     if (temp_tile.terrain_type == T_UNSET) {
    //         f.tiles[i] = make_tile_from_terrain_type(T_OUTSIDE);
    //     }
    // }

    print_debug("\nGenerated Floor:\n");
	for (uint16_t y = 0; y < h; y++) {
        for (uint16_t x = 0; x < w; x++) {
            print_debug("%c", f.tiles[(y*w) + x].sprite);
        }
		print_debug("\n");
	}

    return f;
}

D_Tile get_tile(D_Floor floor, Coordinate coors) {
    /*
        Coor is in y and x
        so Tile is in position (y * width) + x
    */
    return floor.tiles[(coors.y * floor.floor_width) + coors.x];
}

