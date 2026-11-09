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
    f.tiles = malloc((uint32_t)w * h * sizeof(D_Tile));

    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
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
    uint16_t mid_x;
    uint16_t mid_y;
} D_Room;

D_Floor generate_floor(uint16_t h, uint16_t w, uint16_t room_tries) {
    D_Floor f = {
        .floor_height = h,
        .floor_width = w
    };

    //Allocate memory for a tile array
    f.tiles = malloc((uint32_t) w * h * sizeof (D_Tile));

    /*
        For 0 to room_tries
        Spawn a room with at random coordinates (a, b) with variable (h, w)
        Check if that room is inbound and doesn't intersect with another room 
        If ok, create tiles and save to floor
    */


    for (uint32_t i = 0; i < (h * w); i++) {
        f.tiles[i] = make_tile_from_terrain_type(T_OUTSIDE);
    }

    //Max possible rooms is if all rooms are valid
    D_Room *rooms = malloc(room_tries * sizeof(D_Room));
    uint16_t room_num = 0;

    for (uint16_t i = 0; i < room_tries; i++) {
        D_Room new_room = {
            .w = get_random_int(3, (uint16_t) w / 4), // picking 4 randomly
            .h = get_random_int(3, (uint16_t) h / 6), // picking 6 randomly
        };

        //Top left corner of the box - needs to shift to account for boundaries
        new_room.x1 = get_random_int(0, w - (new_room.w + 1));
        new_room.x2 = (new_room.x1 + new_room.w); //Do I need to do a -1??
        new_room.y1 = get_random_int(0, h - (new_room.h + 1));
        new_room.y2 = (new_room.y1 + new_room.h); 
        //new
        
        print_debug("\nChecking Room: (y1,x1): (%hu, %hu); (h,w): (%hu, %hu); (y2,x2): %hu, %hu", new_room.y1, new_room.x1, new_room.h, new_room.w, new_room.y2, new_room.x2);

        bool add_room = true;
        /*
            Collision Detection    
        */
        for (int r = 0; r < room_num; r++) {
            
            D_Room temp_room = rooms[r];

            //Do I need to do a -1 to check overlap??
            if ((new_room.x1 <= temp_room.x2) &&
                (new_room.x2 >= temp_room.x1) &&
                (new_room.y1 <= temp_room.y2) &&
                (new_room.y2 >= temp_room.y1)
            ) {
                add_room = false;
                break;
            }
        };

        if (add_room) {
            //add room to rooms
            rooms[room_num] = new_room;
            room_num++;

            //Add to floor (Can separate this out in a diff function later...)
            for (uint16_t j = new_room.y1; j < new_room.y2; j++) {
                for (uint16_t k = new_room.x1; k < new_room.x2; k++) {
                    uint16_t r_index = (j * w) + k;

                    //wall placement checks
                    if (
                        j == new_room.y1 || 
                        j == (new_room.y2 - 1) ||
                        k == new_room.x1 || 
                        k == (new_room.x2 - 1)
                    ) {
                        f.tiles[r_index] = make_tile_from_terrain_type(T_WALL);
                    } else {
                        f.tiles[r_index] = make_tile_from_terrain_type(T_FLOOR);
                    }
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

