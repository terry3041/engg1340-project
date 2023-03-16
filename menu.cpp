#include "config.h"

//A function to print out the interface for the main menu
void menu(int &select, int x, int y) // display menu
{
    erase();
    int y2 = y / 2 - 2;
    int x2 = x / 2 - 4;
    mvprintw(y2 - 7, x2 - 11, "    ____        _     __         ");
    mvprintw(y2 - 6, x2 - 11, "   / __ \\____ _(_)___/ /__  ____ ");
    mvprintw(y2 - 5, x2 - 11, "  / /_/ / __ `/ / __  / _ \\/ __ \\");
    mvprintw(y2 - 4, x2 - 11, " / _, _/ /_/ / / /_/ /  __/ / / /");
    mvprintw(y2 - 3, x2 - 11, "/_/ |_|\\__,_/_/\\__,_/\\___/_/ /_/ ");
    (select == 1) ? mvprintw(y2, x2, "< Play >") : mvprintw(y2, x2, "Play");
    (select == 2) ? mvprintw(y2 + 1, x2, "< Quit >") : mvprintw(y2 + 1, x2, "Quit");
    mvprintw(y2 + 3, x2 - 14, "Tips: Use WASD or arrow keys to navigate,");
    mvprintw(y2 + 4, x2 - 8, "Space or Enter key to select.");
}

//A function to print out the interface for the try-again menu
void tryMenu(int &select, int x, int y)
{
    erase();
    int y2 = y / 2 - 2;
    int x2 = x / 2 - 4;
    mvprintw(y2 - 7, x2 - 22, "   ______                        ____                 ");
    mvprintw(y2 - 6, x2 - 22, "  / ____/___ _____ ___  ___     / __ \\_   _____  _____");
    mvprintw(y2 - 5, x2 - 22, " / / __/ __ `/ __ `__ \\/ _ \\   / / / / | / / _ \\/ ___/");
    mvprintw(y2 - 4, x2 - 22, "/ /_/ / /_/ / / / / / /  __/  / /_/ /| |/ /  __/ /    ");
    mvprintw(y2 - 3, x2 - 22, "\\____/\\__,_/_/ /_/ /_/\\___/   \\____/ |___/\\___/_/     ");
    mvprintw(y2 - 1, x2 - 1, "Try again?");
    (select == 1) ? mvprintw(y2 + 1, x2, "< Yes >") : mvprintw(y2 + 1, x2, "Yes");
    (select == 2) ? mvprintw(y2 + 2, x2, "< No >") : mvprintw(y2 + 2, x2, "No");
}

//A function for selecting items in the try again menu, with controls
void tryAgain(int &choice, int x, int y)
{
    erase();
    int select = 1;
    bool quit = false;
    int y2 = y / 2 - 2;
    int x2 = x / 2 - 4;
    while (true)
    {
        int ch = getch();
        if (ch == 'w' || ch == KEY_UP)
            select = (select - 1 >= 1) ? select = select - 1 : select;
        else if (ch == 's' || ch == KEY_DOWN)
            select = (select + 1 <= 2) ? select = select + 1 : select;
        else if (ch == '\n' || ch == ' ')
        {
            switch (select)
            {
            case 1:
                choice = 1;
                quit = true;
                break;
            case 2:
                choice = 0;
                quit = true;
                break;
            }
        }
        tryMenu(select, x, y);
        if (quit)
            break;
        usleep(5000);
    }
}

//A function for selecting items in the main menu, with controls
void menuSelect(int &select, int x, int y)
{
    bool quit;
    while (true)
    {
        int ch = getch();
        if (ch == 'w' || ch == KEY_UP)
            select = (select - 1 >= 1) ? select = select - 1 : select;
        else if (ch == 's' || ch == KEY_DOWN)
            select = (select + 1 <= 2) ? select = select + 1 : select;
        else if (ch == '\n' || ch == ' ')
        {
            switch (select)
            {
            case 1:
                gameBoard(x, y);
                break;
            case 2:
                quit = true;
                break;
            }
            break;
        }
        menu(select, x, y);
        if (quit)
            break;
        usleep(5000);
    }
    erase();
    endwin();
}
