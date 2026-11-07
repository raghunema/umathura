#include <ncurses.h>
#include "io.h"

int get_play_input(void) {
    int ch = getch();
    return ch;
}