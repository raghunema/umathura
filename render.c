#include "render.h"
#include <ncurses.h>
#include <string.h>
#include <stdio.h>

WINDOW *play_win;
WINDOW *status_win;
WINDOW *menu_win;

int debug_row = 1;
int play_win_h;
int play_win_w;

static FILE *debug_file = NULL;

void reset_debug_row(void){
	debug_row = 1;
}

void debug_init(const char *path) {
	debug_file = fopen(path, "w");
	if (debug_file) setvbuf(debug_file, NULL, _IOLBF, 0); 
}

void print_debug(const char *fmt, ...) {
    // char buf[128];

    // va_list args;
    // va_start(args, fmt);
    // vsnprintf(buf, sizeof buf, fmt, args);
    // va_end(args);

	// fflush(debug_file);
    // //mvwprintw(status_win, debug_row, 2, "%-40s", buf);
    // //wrefresh(status_win);
	// debug_row++;

	if (!debug_file) return;

    va_list args;
    va_start(args, fmt);
    vfprintf(debug_file, fmt, args);
    va_end(args);

    fflush(debug_file);
}

void debug_close(void) {
    if (debug_file) fclose(debug_file);
    debug_file = NULL;
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

	print_debug("Play_Win (H, W): (%d, %d)\n\n", play_win_h, play_win_w);

    return 0;
}

int initialize_character_placement(Player *p) {
    mvwaddch(play_win, (int) p->coors.y, (int) p->coors.x, p->sprite);
    wrefresh(play_win);

    return 0;
}

int end_ncurses(void) {
    delwin(play_win);
	delwin(status_win);
	delwin(menu_win);

	endwin();			/* End curses mode		  */
    return 0;
}

Camera initalize_win_camera(Coordinate start_coors) {
	char *camera_view = malloc((uint32_t)play_win_h * play_win_w * sizeof(char));

	Coordinate win_midpoint = {
		.y = (uint16_t) play_win_h / 2,
		.x = (uint16_t) play_win_w / 2
	};

	Camera c = {
		.map_position = start_coors,
		.win_midpoint = win_midpoint,
		.max_distance_from_player = 3,
		.win_h = play_win_h,
		.win_w = play_win_w,
		.camera_view = camera_view
	};

	return c;
}

void update_camera_position(Player *p, Camera *c) {
	uint16_t distance_y = (p->coors.y > c->map_position.y) ? p->coors.y - c->map_position.y : c->map_position.y - p->coors.y;
	uint16_t distance_x = (p->coors.x > c->map_position.x) ? p->coors.x - c->map_position.x : c->map_position.x - p->coors.x;

	if (distance_y > c->max_distance_from_player) {
		if (p->coors.y > c->map_position.y) {
			c->map_position.y++;
		} else {
			c->map_position.y--;
		}
	}
	
	if (distance_x > c->max_distance_from_player) {
		if (p->coors.x > c->map_position.x) {
			c->map_position.x++;
		} else {
			c->map_position.x--;
		}
	}

}

//Write a view into camera to then render
void set_camera_view(Camera *c, D_Floor *f, Player *p) {

	//resetting camera_view
	werase(play_win);

	/* 
		Camera is at position (3,3)
		play_win d = (54, 201)
		play_win midpoint = (27, 100)

		Need to shit (3,3) and render it at (27,100)
	*/

	Coordinate shift_coors_by = {
		.y = c->map_position.y - c->win_midpoint.y,
		.x = c->map_position.x - c->win_midpoint.x
	};

	/*
		1. Loop through only the coordinates the camera can see
		2. Get tiles
		3. Shift tiles 
		4. Render immediately?
	*/

	/* 
		For the camera at (a, b), get view bounds
		min_x = (a - 1/2 camera_w) || 0
		max_x = (a + 1/2 camera_w) || floor_w
		min_y = (b - 1/2 camera_h) || 0
		max_y = (b + 1/2 camera_h) || floor_h
	*/

	int32_t min_x = c->map_position.x - (c->win_w / 2);
	min_x = (min_x > 0) ? min_x : 0;

	int32_t max_x = c->map_position.x +((c->win_w / 2) - 1);
	max_x = (max_x < (int32_t) f->floor_width) ? max_x : f->floor_width;

	int32_t min_y = c->map_position.y - (c->win_h / 2);
	min_y = (min_y > 0) ? min_y : 0;

	int32_t max_y = c->map_position.y + ((c->win_h / 2) - 1);
	max_y = (max_y < (int32_t) f->floor_height) ? max_y : f->floor_height;

	print_debug("\nMin_X: %d, Max_x: %d, Min_Y: %d, Max_Y: %d\n", min_x, max_x, min_y, max_y);

	for (int32_t i = min_y; i < max_y; i++) {
		for (int32_t j = min_x; j < max_x; j++) {

			Coordinate org_tile_coor = {
				.y = i,
				.x = j
			};

			uint32_t f_index = flat_poistion_from_coordinate(org_tile_coor, f->floor_width, f->floor_height);
			char sprite = f->tiles[f_index].sprite;

			Coordinate new_tile_coor = {
				.y = org_tile_coor.y - shift_coors_by.y,
				.x = org_tile_coor.x - shift_coors_by.x
			};

			mvwaddch(play_win, new_tile_coor.y, new_tile_coor.x, sprite);

		}
	}

	Coordinate player_shifted_coor = {
		.y = p->coors.y - shift_coors_by.y,
		.x = p->coors.x - shift_coors_by.x
	};

	mvwaddch(play_win, player_shifted_coor.y, player_shifted_coor.x, p->sprite);

	wrefresh(play_win);

}
