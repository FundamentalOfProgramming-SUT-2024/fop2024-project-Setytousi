#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

int CUR_USER;
FILE *users_ptr, *emails_ptr, *passwords_ptr, *levels_ptr;

void draw_border_menu();
int check_username(char *username);
int check_password(char *password);
int check_email(char *email);
void new_user_menu();
void menus();
void close_files();
void open_files();



int main(){
    open_files();
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    if (has_colors()){
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_WHITE);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_WHITE);
        init_pair(4, COLOR_RED, COLOR_BLACK);
    }
    else return 0;
    menus();
    while(1){

    }
    close_files();
}


void draw_border_menu(){
    attron(COLOR_PAIR(1));
    for (int i = 0; i <= 20; i++){
        mvprintw(i, COLS / 2 - 30, " ");
        mvprintw(i, COLS / 2 + 30," ");
    }
    for (int i = COLS / 2 - 30; i <= COLS / 2 + 30; i++){
        mvprintw(0, i," ");
        mvprintw(20, i," ");
    }
    refresh();
    attroff(COLOR_PAIR(1));
}
int check_username(char *username){
    char s[30];
    while(fgets(s, 30, users_ptr) != NULL){
        if (strcmp(s, username) == 0) return 0;
    }
    return 1;
}
int check_password(char *password){
    int len = strlen(password);
    if (len < 7) return 0;
    int check_uppercase = 0, check_lowercase = 0, check_num = 0;
    for (int i = 0; i < len; i++){
        if (password[i] >= 'A' && password[i] <= 'Z') check_uppercase = 1;
        else if (password[i] >= 'a' && password[i] <= 'z') check_lowercase = 1;
        else if (password[i] >= '0' && password[i] <= '9') check_num = 1;
    }
    if (check_lowercase && check_num && check_uppercase) return 1;
    return 0;
}
int check_email(char *email){
    int check0 = 0, check_atsign = 0, check1 = 0, check_dot = 0, check2 = 0;
    int len = strlen(email);
    for (int i = 0; i < len; i++){
        if (check_dot) check2 = 1;
        if (check1 && email[i] == '.') check_dot = 1;
        if (check_atsign) check1 = 1;
        if (check0 && email[i] == '@') check_atsign = 1;
        check0 = 1;
    }
    return check2;
}
void new_user_menu(){
    char username[30];
    char email[30];
    char password[30];
    //get username
    echo();
    {
        draw_border_menu();
        attron(COLOR_PAIR(2));
        attron(A_BLINK);
        mvprintw(3, COLS / 2 - 20, "Enter username:");
        attroff(COLOR_PAIR(2));
        attroff(A_BLINK);
        refresh();
        scanw("%s", username);
        if (!check_username(username)){
            clear();
            attron(COLOR_PAIR(4));
            attron(A_BLINK);
            mvprintw(3, COLS / 2 - 15, "Username already exists");
            attroff(COLOR_PAIR(4));
            attroff(A_BLINK);
            refresh();
            sleep(2);
            clear();
            noecho();
            menus();
            return;
        }
    }
    //get password
    clear();
    {
        while(1){
            draw_border_menu();
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(3, COLS / 2 - 20, "Enter password:");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
            refresh();
            scanw("%s", password);
            if (!check_password(password)){
                clear();
                attron(COLOR_PAIR(4));
                attron(A_BLINK);
                mvprintw(3, COLS / 2 - 25, "Password should be longer than 7 characters and should contain at least one number, one lowercase letter and one uppercase letter");
                attroff(COLOR_PAIR(4));
                attroff(A_BLINK);
                refresh();
                sleep(2);
                clear();
            }
            else break;
        }
    }
    clear();
    //get email
        {
        while(1){
            draw_border_menu();
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(3, COLS / 2 - 20, "Enter email:");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
            refresh();
            scanw("%s", email);
            if (!check_email(email)){
                clear();
                attron(COLOR_PAIR(4));
                attron(A_BLINK);
                mvprintw(3, COLS / 2 - 25, "Enter valid email");
                attroff(COLOR_PAIR(4));
                attroff(A_BLINK);
                refresh();
                sleep(2);
                clear();
            }
            else break;
        }
    }
    attron(COLOR_PAIR(2));
    mvprintw(3, COLS / 2 - 20, "Account created. Please Login !");
    refresh();
    sleep(2);
    attroff(COLOR_PAIR(2));
    noecho();
    // add_user(username, password, email);
    menus();
}

void menus(){
    clear();
    int curmenu = 0;
    int total_menus = 5;
    while(1){
        clear();
        attron(A_BOLD);
        attron(A_BLINK);
        attron(COLOR_PAIR(2));
        mvprintw(4, (COLS - 5) / 2, "ROGUE");
        attroff(A_BOLD);
        attroff(A_BLINK);
        attroff(COLOR_PAIR(2));
        draw_border_menu();
        if (curmenu == 0){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(6, (COLS - 10) / 2, "Create new user");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(6, (COLS - 10) / 2, "Create new user");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 1){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(7, (COLS - 10) / 2, "Login");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(7, (COLS - 10) / 2, "Login");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 2){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(8, (COLS - 10) / 2, "Guest User");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(8, (COLS - 10) / 2, "Guest User");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 3){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(9, (COLS - 10) / 2, "Scoreboard");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(9, (COLS - 10) / 2, "Scoreboard");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 4){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(10, (COLS - 10) / 2, "Exit");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(10, (COLS - 10) / 2, "Exit");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        int ch = getch();
        if (ch == KEY_UP){
            curmenu--;
            curmenu = (curmenu + total_menus) % total_menus;
        }
        else if (ch == KEY_DOWN){
            curmenu++;
            curmenu %= total_menus;
        }
        else if (ch == 10){
            if (curmenu == 0){
                clear();
                new_user_menu();
                return;
            }
            else if (curmenu == 1){
                // login_menu();
                return;
            }
            else if (curmenu == 2){
                CUR_USER = -1;
                clear();
                return;
            }
            else if (curmenu == 3){
                // scoreboard();
                return;
            }
            else{
                endwin();
                exit(0);
            }
        }
        else{
            attron(COLOR_PAIR(4));
            mvprintw(1, COLS / 2 - 29, "Invalid Key !");
            refresh();
            sleep(1);
            attroff(COLOR_PAIR(4));
        }
        refresh();
    }
}
void open_files(){
    users_ptr = fopen("users.txt", "w+");
    levels_ptr = fopen("levels.txt", "w+");
    passwords_ptr = fopen("passwords.txt", "w+");
    emails_ptr = fopen("emails.txt", "w+");
}
void close_files(){
    fclose(users_ptr);
    fclose(levels_ptr);
    fclose(passwords_ptr);
    fclose(emails_ptr);
}
