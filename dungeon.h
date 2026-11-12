#pragma once //compile once?
#include <stdbool.h>
#include <stdlib.h>
#include "helpers.h"

static const uint8_t MAX_FLOORS = 255;

//T_OUTSIDE for out of bounds?? 
typedef enum {
    T_UNSET = 0, //for 0-unitialized memory??
    T_OUTSIDE,
    T_FLOOR,
    T_WALL,
    T_COUNT
} Terrain_Type;

// typedef struct {
//     Terrain_Type terrain_type;
//     char sprite;
//     bool is_blocker; //can not move onto something with the same collision layer
// } D_Terrain;


/*
Tiles can have: 
    - Terrain (immoveable/collision-enabled objects)
    - Object (interactable)
    - NPC/Monster (moveable, interactable, take actions)
*/
typedef struct {
   Terrain_Type terrain_type; 
   bool is_blocker;
   char sprite;
   bool is_visible; //True for all right now
} D_Tile; //One tile of the floor

//a dungenon is made of floors
typedef struct { 
    uint32_t floor_height;
    uint32_t floor_width;
    D_Tile *tiles;
} D_Floor; //One floor of a Dungeon

typedef struct {
    int num_floors; //how many floors the dungeon currently has
    int curr_floor;
    D_Floor floors[MAX_FLOORS]; //array of floors
} Dungeon;

/*
    FUNCTIONS
*/

//D_Tile make_tile_from_terrain_type(Terrain_Type terrain_type);
D_Floor floor_from_text(const char *char_map, uint32_t h, uint32_t w);
D_Tile get_tile(D_Floor *floor, Coordinate coors);

D_Floor generate_floor(Coordinate *start_coors, uint16_t h, uint16_t, uint16_t room_tries);