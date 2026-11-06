#pragma once //compile once?

#include "helpers.h"

typedef struct {
    Coordinates coors;
    char sprite;
} Player;

Player intialize_player(Coordinates start_coors);
int initialize_character_placement(Player *p);

// move_char = getch()) != 'q') {	
		
// 		mvwaddch(play_win, char_y, char_x, erase_char); 

//         switch (move_char)
//         {
//         case KEY_DOWN:
//             ++char_y;
//             break;
//         case KEY_UP:
//             --char_y;
//             break;
//         case KEY_LEFT:
//             --char_x;
//             break;
//         case KEY_RIGHT:
//             ++char_x;
//             break;
//         }

//         mvwaddch(play_win, char_y, char_x, character);
//         wrefresh(play_win);