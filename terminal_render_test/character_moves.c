#include <ncurses.h>
#include <stdio.h>
#include <string.h>

int main(void) {

    initscr(); //start ncurses mode
    
    int char_y = 10, char_x = 10; //character positions - start at(10, 10)
    int move;
    char character = '@';
    char erase_char = ' ';
    keypad(stdscr, TRUE);

    cbreak(); //don't need to press enter to process input
    noecho(); //don't write normal key presses at cursor position
    curs_set(0); //hide cursor

    mvwaddch(stdscr, char_y, char_x, character);
    refresh();

    while ((move = getch()) != KEY_F(1)) {

        mvwaddch(stdscr, char_y, char_x, erase_char); 

        switch (move)
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

        mvwaddch(stdscr, char_y, char_x, character);
        refresh();
        
    }

    endwin(); //end nucurses mode

    return 0;
}