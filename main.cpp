#include "config.h"

bool quit = false;

int main()
{
    initscr();             // initialize ncurses
    cbreak();              // disable line buffering
    noecho();              // disable echoing of getch()
    nodelay(stdscr, TRUE); // disable getch() blocking
    keypad(stdscr, TRUE);  // enable keypad mode
    curs_set(0);           // hide cursor
    srand(time(NULL));

    // initialize color
    use_default_colors();
    start_color();
    init_pair(1, COLOR_BLACK, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_RED, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(5, COLOR_CYAN, -1);

    int x, y, select = 1;
    getmaxyx(stdscr, y, x); // get the screen size
    int maxX = y * 1.5;     // store the order size
    int maxY = y;
    resizeterm(maxY, maxX);
    menu(select, maxX, maxY); //call the functions
    menuSelect(select, maxX, maxY);

    return 0;
}
