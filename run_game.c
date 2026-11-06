#include <ncurses.h>
#include <stdlib.h>

#include "dungeon.h"

//Split terminal into three sections i.e. windows
// 1. Map/Play window  
	// H: 75% W: 75%
// 2. Health/Statuses etc
	// H: 75% W: 25% (last quarter)
// 3. Menu + instructuins 
	// H: Bottm 25% W: 100%


const char *test_map = 
    "##########"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "#........#"
    "##########"
    ;


WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);
	wrefresh(local_win);
	return local_win;
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

void render_map(const Dungeon *d, WINDOW *win) {
    D_Floor curr_floor = d->floors[d->curr_floor];
    size_t h = curr_floor.floor_height;
    size_t w = curr_floor.floor_width;
   
    /*
        D_Floor.tiles is a flat map
        [Width, Width, Width]
    */
    for (size_t i = 0; i < (size_t) w * h; i++) {
        size_t row = i % w;
        size_t col = i - (row * w);

        D_Tile curr_tile = curr_floor.tiles[i];
        mvwaddch(win, row, col, curr_tile.sprite);
    }

}

int main(void)
{	WINDOW *play_win;
	WINDOW *status_win;
	WINDOW *menu_win;

	int startx, starty, width, height;

	initscr();			/* Start curses mode 		*/
	cbreak();			// Not needed tbh
	keypad(stdscr, TRUE);		
	
	cbreak();
	noecho();
	curs_set(0); //hide cursor

	refresh();

	starty = 0; startx = 0;
	height = (int) (LINES * 0.75); 
	width = (int) (COLS * 0.75); 
	play_win = create_newwin(height, width, starty, startx);

	//start y stays the same
	startx = width; 
	height = (int) (LINES * 0.75); 
	width = (int) (COLS - width);
	status_win = create_newwin(height, width, starty, startx);

	starty = height; 
	startx = 0; //reset to 0
	height = (int) (LINES - height);
	width = COLS; 
	menu_win = create_newwin(height, width, starty, startx);
	
	mvwprintw(menu_win, 1, 1, " Arrows keys for movement");
	mvwprintw(menu_win, 2, 1, " q to Quit");
	wrefresh(menu_win);

    Dungeon d;
    d.num_floors = 1;
    d.curr_floor = 0;
    d.floors[0] = floor_from_text(test_map, 10, 10);

    render_map(&d, play_win);

	int move_char;
    char character = '@';
    char erase_char = ' ';

	int char_y = 10, char_x = 10;
	mvwaddch(play_win, char_y, char_x, character);
	wrefresh(play_win);
	
	while((move_char = getch()) != 'q') {	
		
		mvwaddch(play_win, char_y, char_x, erase_char); 

        switch (move_char)
        {
        case KEY_DOWN:
            ++char_y;
            break;
        case KEY_UP:
            --char_y;
            break;
        case KEY_LEFT:
            --char_x;
            break;
        case KEY_RIGHT:
            ++char_x;
            break;
        }

        mvwaddch(play_win, char_y, char_x, character);
        wrefresh(play_win);

	}

	delwin(play_win);
	delwin(status_win);
	delwin(menu_win);

	endwin();			/* End curses mode		  */
	
	return 0;
}

