#pragma once

#include <stdlib.h>

typedef struct {
    size_t x;
    size_t y;
} Coordinate;

Coordinate get_new_coors_from_input(Coordinate prev_coors, int ch);
Coordinate coordinate_from_flat_map(size_t i, size_t w, size_t h);
size_t flat_poistion_from_coordinate(Coordinate coor, size_t w, size_t h);