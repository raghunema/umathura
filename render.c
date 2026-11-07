#include "render.h"
#include <ncurses.h>

WINDOW *play_win;
WINDOW *status_win;
WINDOW *menu_win;

int debug_row = 1;
int play_win_h;
int play_win_w;

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

	play_win_h = height;
	play_win_w = width;

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

	print_debug("Play_Win (H, W): (%d, %d)", play_win_h, play_win_w);

    return 0;
}

int initialize_character_placement(Player *p) {
    mvwaddch(play_win, (int) p->coors.y, (int) p->coors.x, p->sprite);
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

Camera initalize_win_camera(Coordinate start_coors) {
	char *camera_view = malloc((size_t)play_win_h * play_win_w * sizeof(char));

	Coordinate win_midpoint = {
		.y = (size_t) play_win_h / 2,
		.x = (size_t) play_win_w / 2
	};

	Camera c = {
		.map_position = start_coors,
		.win_midpoint = win_midpoint,
		.max_distance_from_player = 10,
		.win_h = play_win_h,
		.win_w = play_win_w,
		.camera_view = camera_view
	};

	return c;
}

//Write a view into camera to then render
void set_camera_view(Camera *c, D_Floor *f, Player *p) {

	/* 
		Camera is at position (3,3)
		play_win d = (54, 201)
		play_win midpoint = (27, 100)

		Need to shit (3,3) and render it at (27,100)
	*/

	size_t floor_h = f->floor_height;
    size_t floor_w = f->floor_width;
	
	size_t shift_y = c->map_position.y - c->win_midpoint.y;
	size_t shift_x = c->map_position.x - c->win_midpoint.x;
	
	/* 
		For tile at position (y, x) get new camera position of (y', x')
		Store the sprite into camera view
	*/
	for (size_t i = 0; i < floor_h * floor_w; i++) {
		/*
			For Each tiles in flat map of the floor
			Convert Tile into Floor-Coorindate
			Shift Cooridnate for Map
			Turn back into flat position
			Add into camera view
		*/	
		
		Coordinate global_coor = coordinate_from_flat_map(i, floor_w, floor_h);
		
		//Should handle negative shifts?? need to confirm!!
		Coordinate shifted_coor = {
			.y = global_coor.y - shift_y,
			.x = global_coor.x - shift_x
		};

		size_t new_flat_position = flat_poistion_from_coordinate(shifted_coor, c->win_w, c->win_h);
		c->camera_view[new_flat_position] = f->tiles[i].sprite;
	}

}

void render_camera_view(Camera *c) {
	size_t h_cam = c->win_h;
	size_t w_cam = c->win_w;

	for (size_t i = 0; i < h_cam * w_cam; i++) {
		Coordinate camera_view_coor = coordinate_from_flat_map(i, w_cam, h_cam);
		mvwaddch(play_win, camera_view_coor.y, camera_view_coor.x, c->camera_view[i]);
	}
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
