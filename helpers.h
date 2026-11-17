#pragma once

#include <stdlib.h>

#define MAX_ARMOR_ITEMS 5

typedef struct {
    uint16_t x;
    uint16_t y;
} Coordinate;

//i.e. 2d6
typedef struct {
    uint8_t n;
    uint8_t d;
} Die;

typedef struct {
    char *name;
    uint8_t weight; //incumberance??
    uint8_t ac_mod;
    Die dmg_die;
} Item;

Coordinate get_new_coors_from_input(Coordinate prev_coors, int ch);
Coordinate coordinate_from_flat_map(uint32_t i, uint16_t w, uint16_t h);
uint32_t flat_poistion_from_coordinate(Coordinate coor, uint16_t w, uint16_t h);

void set_seed(uint32_t seed);
uint32_t get_random_int(uint32_t min_val, uint32_t max_val);