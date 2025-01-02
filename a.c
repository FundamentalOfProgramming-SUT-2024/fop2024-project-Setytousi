#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

void draw_border(){
    attron(COLOR_PAIR(1));
    for (int i = 0; i < LINES; i++){
        mvprintw(i, 0, " ");
        mvprintw(i, COLS - 1," ");
    }
    for (int i = 0; i < COLS; i++){
        mvprintw(0, i," ");
        mvprintw(LINES - 1, i," ");
    }
    refresh();
    attroff(COLOR_PAIR(1));
}

void menus(){
    draw_border();
    attron(A_BOLD);
    attron(A_BLINK);
    attron(COLOR_PAIR(2));
    mvprintw(3, (COLS - 5) / 2, "ROGUE");
    attroff(A_BOLD);
    attroff(A_BLINK);
    int curmenu = 0;
    int total_menus = 5;
    while(1){
        mvprintw(3, (COLS - 10) / 2, "Create new user");
        mvprintw(4, (COLS - 10) / 2, "Create new user");
        mvprintw(5, (COLS - 10) / 2, "Create new user");
        mvprintw(6, (COLS - 10) / 2, "Create new user");
        int ch = getch();
        if (ch == KEY_DOWN && curmenu < total_menus - 1){

        }
    }
    refresh();
}


int main(){
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    if (has_colors()){
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_WHITE);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    }
    else return 0;
    menus();
    while(1){

    }
}