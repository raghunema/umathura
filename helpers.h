#pragma once

#include <stdlib.h>

typedef struct {
    size_t x;
    size_t y;
} Coordinate;

Coordinate get_new_coors_from_input(Coordinate prev_coors, int ch);