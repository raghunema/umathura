#include <ncurses.h>

//Split terminal into three sections i.e. windows
// 1. Map/Play window  
	// H: 75% W: 75%
// 2. Health/Statuses etc
	// H: 75% W: 25% (last quarter)
// 3. Menu + instructuins 
	// H: Bottm 25% W: 100%


WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);
	wrefresh(local_win);
	return local_win;
}


int main(void)
{	WINDOW *play_win;
	WINDOW *status_win;
	WINDOW *menu_win;

	int startx, starty, width, height;

	initscr();			/* Start curses mode 		*/
	cbreak();			// Not needed tbh
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	
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
	
	mvwprintw(menu_win, 1, 1, "Arrows keys for movement");
	mvwprintw(menu_win, 2, 1, "q to Quit");
	wrefresh(menu_win);



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

