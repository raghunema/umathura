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

D_Tile get_tile(D_Floor *floor, Coordinate coors) {
    /*
        Coor is in y and x
        so Tile is in position (y * width) + x
    */
    return floor->tiles[(coors.y * floor->floor_width) + coors.x];
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

//Convert these all into coordinates??
typedef struct {
    Coordinate min;
    Coordinate max;
    Coordinate mid;
    uint16_t w; 
    uint16_t h;
} D_Room;

D_Room generate_random_room(uint16_t w, uint16_t h) {
    D_Room new_room = {
        .w = get_random_int(3, (uint16_t) w / 4), // picking 4 randomly
        .h = get_random_int(3, (uint16_t) h / 6), // picking 6 randomly
    };

    //Top left corner of the box - needs to shift to account for boundaries
    new_room.min.x = get_random_int(0, w - (new_room.w + 1));
    new_room.max.x = (new_room.min.x + new_room.w); //Do I need to do a -1??
    new_room.min.y = get_random_int(0, h - (new_room.h + 1));
    new_room.max.y = (new_room.min.y + new_room.h); 
    new_room.mid.x = (uint16_t) (new_room.min.x + (new_room.w / 2)); //Do I need to do a -1 here as well?
    new_room.mid.y = (uint16_t) (new_room.min.y + (new_room.h / 2));

    return new_room;
}

void create_room_tiles(D_Floor *f, D_Room *room) {

    for (uint16_t j = room->min.y; j < room->max.y; j++) {
        for (uint16_t k = room->min.x; k < room->max.x; k++) { //I think this '<' operator takes care of the -1 offset of room wide
            uint16_t r_index = (j * f->floor_width) + k;

            //wall placement checks - skip for now and create walls later
            if (
                j == room->min.y || 
                j == (room->max.y - 1) ||
                k == room->min.x || 
                k == (room->max.x - 1)
            ) {
                continue;
                //f->tiles[r_index] = make_tile_from_terrain_type(T_WALL);
            } else {
                f->tiles[r_index] = make_tile_from_terrain_type(T_FLOOR);
            }
        }
    }

}

uint16_t tunnel_horizontal(D_Floor *f, uint16_t y, uint16_t x1, uint16_t x2) {

    uint16_t row = y * f->floor_width;

    if (x1 < x2) {
        for (int i = x1; i < x2; i++) {
            f->tiles[row + i] = make_tile_from_terrain_type(T_FLOOR);
        }

        print_debug("Horizontal Tunnel from (%hu, %hu) at %hu(y)\n", x1, x2, y);
        return x2;
    } else {
        for (int i = x2; i < x1; i++) {
            f->tiles[row + i] = make_tile_from_terrain_type(T_FLOOR);
        }

        print_debug("Horizontal Tunnel from (%hu, %hu) at %hu(y)\n", x2, x1, y);
        return x1;
    }
}

uint16_t tunnel_vertical(D_Floor *f, uint16_t x, uint16_t y1, uint16_t y2) {

    if (y1 < y2) {
        for (int i = y1; i < y2; i++) {
            uint16_t row = i * f->floor_width;
            f->tiles[row + x] = make_tile_from_terrain_type(T_FLOOR);
        }

        print_debug("Vertical Tunnel from (%hu, %hu) at %hu(x)\n", y1, y2, x);
        return y2;
    } else {
        for (int i = y2; i < y1; i++) {
            uint16_t row = i * f->floor_width;
            f->tiles[row + x] = make_tile_from_terrain_type(T_FLOOR);
        }

        print_debug("Vertical Tunnel from (%hu, %hu) at %hu(x)\n", y2, y1, x);
        return y1;
    }
}

void place_walls(D_Floor *f) {
    /*
        If a tile has a floor next to it
            But has less than 4 floors next to it
            Place a wall
        What if a tile has a wall next to it?
            Idk
    */
    uint16_t h = f->floor_height;
    uint16_t w = f->floor_width;

    for (uint32_t i = 0; i < h * w; i++) {
        Coordinate curr_coor = coordinate_from_flat_map(i, w, h);
        D_Tile curr_tile = get_tile(f, curr_coor);

        if (curr_tile.terrain_type == T_FLOOR) {
            continue;
        }

        D_Tile neighbors[4];
        size_t num_floor_neighbors = 0;

        if ((curr_coor.y - 1) >= 0) {
            Coordinate above = { .x = curr_coor.x, .y = (curr_coor.y - 1)};
            neighbors[0] = get_tile(f, above);
            if (neighbors[0].terrain_type == T_FLOOR) {
                num_floor_neighbors++;
            }
        }
        
        if ((curr_coor.x + 1) != w) {
            Coordinate right = { .x = (curr_coor.x + 1), .y = curr_coor.y};
            neighbors[1] = get_tile(f, right);
            if (neighbors[1].terrain_type == T_FLOOR) {
                num_floor_neighbors++;
            }
        }

        if ((curr_coor.y + 1) != h) {
            Coordinate below = { .x = curr_coor.x, .y = (curr_coor.y + 1)};
            neighbors[2] = get_tile(f, below);
            if (neighbors[2].terrain_type == T_FLOOR) {
                num_floor_neighbors++;
            }
        }
        
        if ((curr_coor.x - 1) >= 0) {
            Coordinate left = { .x = (curr_coor.x - 1), .y = curr_coor.y};
            neighbors[3] = get_tile(f, left);
            if (neighbors[3].terrain_type == T_FLOOR) {
                num_floor_neighbors++;
            }
        };
        
        if (num_floor_neighbors > 0 && num_floor_neighbors < 4) {
            f->tiles[i] = make_tile_from_terrain_type(T_WALL);
        }
    }
}

D_Floor generate_floor(Coordinate *start_coors, uint16_t h, uint16_t w, uint16_t room_tries) {
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
        D_Room new_room = generate_random_room(f.floor_width, f.floor_height); 
        
        //print_debug("\nChecking Room: (y1,x1): (%hu, %hu); (h,w): (%hu, %hu); (y2,x2): %hu, %hu", new_room.min.y, new_room.min.x, new_room.h, new_room.w, new_room.max.y, new_room.max.x);

        bool add_room = true;
        /*
            Collision Detection    
                Do I need to add collision for the tunnels?? (probably :( )
        */
        for (int r = 0; r < room_num; r++) {
            
            D_Room temp_room = rooms[r];

            //Do I need to do a -1 to check overlap??
            if ((new_room.min.x <= temp_room.max.x) &&
                (new_room.max.x >= temp_room.min.x) &&
                (new_room.min.y <= temp_room.max.y) &&
                (new_room.max.y >= temp_room.min.y)
            ) {
                add_room = false;
                break;
            }
        };

        if (add_room) {
            //Add to floor (Can separate this out in a diff function later...)
            create_room_tiles(&f, &new_room);

            /*
                Connect this new_room with the last previously generated room
            */
            if (room_num > 0) {
                D_Room prev_rooom = rooms[room_num - 1];

                /*
                    How to find the closest coordinates of these two rooms? 
                    Which two points am I connecting??
                        Maybe I can use that bresenham distance or something
                        No - I would loop over all the Wall Tiles of each room (n^2) and calculate the L-1 Distance 
                        Find the closest pair of coordinates and make a tunnel between them
                        There's gotta be a faster way lmao
                    Lets just start with the center
                */
                
                print_debug("Room 1:(%hu, %hu); Room 2: (%hu, %hu)\n", prev_rooom.mid.y, prev_rooom.mid.x, new_room.mid.y, new_room.mid.x);            
                uint16_t x_pos = tunnel_horizontal(&f, prev_rooom.mid.y, prev_rooom.mid.x, new_room.mid.x);
                uint16_t y_pos = tunnel_vertical(&f, x_pos, prev_rooom.mid.y, new_room.mid.y);
                print_debug("\n");
            } else {
                start_coors->x = new_room.mid.x;
                start_coors->y = new_room.mid.y;
            }

            //add room to rooms
            rooms[room_num] = new_room;
            room_num++;
        }

    }

    place_walls(&f);

    print_debug("\nGenerated Floor:\n");
	for (uint16_t y = 0; y < h; y++) {
        for (uint16_t x = 0; x < w; x++) {
            print_debug("%c", f.tiles[(y*w) + x].sprite);
        }
		print_debug("\n");
	}

    return f;
}


