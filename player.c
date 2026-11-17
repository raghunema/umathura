#include "player.h"
#include "helpers.h"
#include "render.h"

Player intialize_player(Coordinate start_coors) {
    Item basic_sword = {
        .name = "Basic Sword",
        .dmg_die = { 
            .n = 1,
            .d = 4
        },
    };

    Player p = {
        .coors = start_coors,
        .sprite = '@',
        .level = 1,
        .xp = 0,
        .movement = 1,
        .ac = 0,
        .max_hp = 10,
        .hp = 10,
        .weapon = basic_sword
    };

    p.armor = malloc((uint16_t) MAX_ARMOR_ITEMS * sizeof(Item));

    Item basic_armor = {
        .name = "Basic Armor",
        .ac_mod = 2,
    };

    p.armor[0] = basic_armor;
    
    return p;
};

void update_character_position(Player *p, Coordinate new_coors) {
    p->coors = new_coors;
}