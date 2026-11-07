#include "render.h"
#include <ncurses.h>

WINDOW *play_win;
WINDOW *status_win;
WINDOW *menu_win;

int debug_row = 1;

void reset_debug_row(void){
	debug_row = 1;
}

void print_debug(const char *fmt, ...) {
    char buf[128];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof buf, fmt, args);
    va_end(args);

    mvwprintw(status_win, debug_row, 2, "%-40s", buf);
    wrefresh(status_win);
	debug_row++;
}

int start_ncurses (void) {
    initscr();			/* Start curses mode 		*/
	cbreak();			// Not needed tbh
	keypad(stdscr, TRUE);	

	cbreak();
	noecho();
	curs_set(0); //hide cursor
    
    return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);
	wrefresh(local_win);
	return local_win;
}


/*
Split terminal into three sections i.e. windows
1. Map/Play window  
	H: 75% W: 75%
2. Health/Statuses etc
	H: 75% W: 25% (last quarter)
3. Menu + instructuins 
	H: Bottm 25% W: 100%
*/
int create_screens() {
    int startx, starty, width, height;
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

    return 0;
}

int initialize_character_placement(Player *p) {
    mvwaddch(play_win, p->coors.y, p->coors.x, p->sprite);
    wrefresh(play_win);

    return 0;
}

int render_move_character(Player *p, Coordinate prev_coors, D_Tile prev_tile) {
	//overwrite previous position with tile sprite
	mvwaddch(play_win, prev_coors.y, prev_coors.x, prev_tile.sprite);
	//overwrite new position with character sprite
	mvwaddch(play_win, p->coors.y, p->coors.x, p->sprite);
	wrefresh(play_win);
	return 0;
};

int end_ncurses(void) {
    delwin(play_win);
	delwin(status_win);
	delwin(menu_win);

	endwin();			/* End curses mode		  */
    return 0;
}

void render_map(const Dungeon *d) {
    D_Floor curr_floor = d->floors[d->curr_floor];
    size_t h = curr_floor.floor_height;
    size_t w = curr_floor.floor_width;
   
    /*
        D_Floor.tiles is a flat map
        [Width, Width, Width] etc
    */
    for (size_t i = 0; i < (size_t) w * h; i++) {
        size_t row = (size_t) i / w; //Floor operation with division to get row
        size_t col = i - (row * w);

        D_Tile curr_tile = curr_floor.tiles[i];
        mvwaddch(play_win, row, col, curr_tile.sprite);
    }

}
