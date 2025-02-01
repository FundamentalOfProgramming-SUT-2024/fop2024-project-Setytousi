#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ncurses.h>


int USERNUMBER = -1;
FILE *users_ptr, *emails_ptr, *passwords_ptr, *scores_ptr, *golds_ptr, *games_ptr, *register_times_ptr;
int mark[1000];


typedef struct{
    int sz;
    int x[300];
    int y[300];
}corridor_struct;
typedef struct{
    int x1, y1, x2, y2;
    int number_of_doors;
    int doors_x[10];
    int doors_y[10];
    int vis;
    int other_room[10];
    int in[30][30];
    int secret_door[10];
    //0 = empty 1 = stairs
}room_struct;
typedef struct{
    int number_of_rooms;
    room_struct rooms[10];
    int room_numbers[10];
    int stairs_x;
    int stairs_y;
}level_struct;
typedef struct{
    int x, y;
}player_struct;
typedef struct{
    player_struct player;
    level_struct levels[10];
    int golds;
    int score;
    int level;
    int health;
    int energy;
    int vis_corridors[10][300][300];
}game_struct;


room_struct pre_defined_rooms[10];
corridor_struct pre_defined_corridors[10][10];
game_struct game;


void draw_border_menu();
int check_username(char *username);
int check_password(char *password);
int check_email(char *email);
void new_user_menu();
void menus();
void close_files();
void open_files();
void add_user(char *username, char *password, char *email);
int countlines(FILE *file);
int find_username(char *username);
void login_menu();
int check_correct_password(char* password);
void scoreboard();
void printmx(int cnt, int line);
void draw_border_scoreboard();
void pregame();
void profile();
void new_game();
void make_default_rooms(){
    room_struct room1;
    room1.x1 = 58; room1.y1 = 8;
    room1.x2 = 75; room1.y2 = 16;
    room1.number_of_doors = 3;
    room1.doors_x[0] = 58; room1.doors_x[1] = 69; room1.doors_x[2] = 75;
    room1.doors_y[0] = 11; room1.doors_y[1] = 16; room1.doors_y[2] = 15;
    room1.other_room[0] = 8, room1.other_room[1] = 3, room1.other_room[2] = 1;
    room_struct room2;
    room2.x1 = 90; room2.y1 = 12;
    room2.x2 = 110; room2.y2 = 20;
    room2.number_of_doors = 3;
    room2.doors_x[0] = 90; room2.doors_x[1] = 110; room2.doors_x[2] = 110;
    room2.doors_y[0] = 15; room2.doors_y[1] = 16; room2.doors_y[2] = 14;
    room2.other_room[0] = 0, room2.other_room[1] = 4, room2.other_room[2] = 2;
    room_struct room3;
    room3.x1 = 125; room3.y1 = 10;
    room3.x2 = 145; room3.y2 = 18;
    room3.number_of_doors = 3;
    room3.doors_x[0] = 125; room3.doors_x[1] = 141; room3.doors_x[2] = 145;
    room3.doors_y[0] = 14; room3.doors_y[1] = 18; room3.doors_y[2] = 14;
    room3.other_room[0] = 1, room3.other_room[1] = 7, room3.other_room[2] = 5;
    room_struct room4;
    room4.x1 = 58; room4.y1 = 20;
    room4.x2 = 84; room4.y2 = 28;
    room4.number_of_doors = 3;
    room4.doors_x[0] = 69; room4.doors_x[1] = 78; room4.doors_x[2] = 84;
    room4.doors_y[0] = 20; room4.doors_y[1] = 28; room4.doors_y[2] = 25;
    room4.other_room[0] = 0, room4.other_room[1] = 6, room4.other_room[2] = 4;
    room_struct room5;
    room5.x1 = 100; room5.y1 = 22;
    room5.x2 = 125; room5.y2 = 30;
    room5.number_of_doors = 4;
    room5.doors_x[0] = 100; room5.doors_x[1] = 121; room5.doors_x[2] = 125; room5.doors_x[3] = 115;
    room5.doors_y[0] = 25; room5.doors_y[1] = 30; room5.doors_y[2] = 28; room5.doors_y[3] = 22;
    room5.other_room[0] = 3, room5.other_room[1] = 7, room5.other_room[2] = 5, room5.other_room[3] = 1;
    room_struct room6;
    room6.x1 = 150; room6.y1 = 24;
    room6.x2 = 160; room6.y2 = 32;
    room6.number_of_doors = 3;
    room6.doors_x[0] = 150; room6.doors_x[1] = 156; room6.doors_x[2] = 159;
    room6.doors_y[0] = 28; room6.doors_y[1] = 24; room6.doors_y[2] = 24;
    room6.other_room[0] = 4, room6.other_room[1] = 2, room6.other_room[2] = 8;
    room_struct room7;
    room7.x1 = 70; room7.y1 = 33;
    room7.x2 = 95; room7.y2 = 41;
    room7.number_of_doors = 3;
    room7.doors_x[0] = 70; room7.doors_x[1] = 78; room7.doors_x[2] = 95;
    room7.doors_y[0] = 37; room7.doors_y[1] = 33; room7.doors_y[2] = 39;
    room7.other_room[0] = 8, room7.other_room[1] = 3, room7.other_room[2] = 7;
    room_struct room8;
    room8.x1 = 110; room8.y1 = 35;
    room8.x2 = 130; room8.y2 = 43;
    room8.number_of_doors = 3;
    room8.doors_x[0] = 110; room8.doors_x[1] = 121; room8.doors_x[2] = 130;
    room8.doors_y[0] = 39; room8.doors_y[1] = 35; room8.doors_y[2] = 38;
    room8.other_room[0] = 6, room8.other_room[1] = 4, room8.other_room[2] = 2;
    room_struct room9;
    room9.x1 = 25; room9.y1 = 5;
    room9.x2 = 45; room9.y2 = 15;
    room9.number_of_doors = 3;
    room9.doors_x[0] = 40; room9.doors_x[1] = 45; room9.doors_x[2] = 45;
    room9.doors_y[0] = 15; room9.doors_y[1] = 11; room9.doors_y[2] = 6;
    room9.other_room[0] = 6, room9.other_room[1] = 0, room9.other_room[2] = 5;
    pre_defined_rooms[0] = room1;
    pre_defined_rooms[1] = room2;
    pre_defined_rooms[2] = room3;
    pre_defined_rooms[3] = room4;
    pre_defined_rooms[4] = room5;
    pre_defined_rooms[5] = room6;
    pre_defined_rooms[6] = room7;
    pre_defined_rooms[7] = room8;
    pre_defined_rooms[8] = room9;
    for (int i = 0; i < 9; i++) pre_defined_rooms[i].vis = 0;
    for (int i = 0; i < 9; i++){
        for (int x = 0; x < 30; x++){
            for (int y = 0; y < 30; y++){
                pre_defined_rooms[i].in[x][y] = 0;
            }
        }
    }
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 10; j++){
            pre_defined_rooms[i].secret_door[j] = 0;
        }
    }
}
void make_default_corridors(){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            pre_defined_corridors[i][j].sz = 0;
        }
    }
    corridor_struct c;
    c.sz = 14;
    for (int i = 0; i <= 13; i++) c.x[i] = 76 + i; 
    for (int i = 0; i <= 13; i++) c.y[i] = 15;      
    pre_defined_corridors[0][1] = pre_defined_corridors[1][0] = c;

    c.sz = 3;
    for (int i = 0; i <= 2; i++) c.x[i] = 69;
    for (int i = 0; i <= 2; i++) c.y[i] = 17 + i;
    pre_defined_corridors[0][3] = pre_defined_corridors[3][0] = c;

    c.sz = 12;
    for (int i = 0; i <= 11; i++) c.x[i] = 46 + i;
    for (int i = 0; i <= 11; i++) c.y[i] = 11;
    pre_defined_corridors[0][8] = pre_defined_corridors[8][0] = c;

    c.sz = 14;
    for (int i = 0; i <= 13; i++) c.x[i] = 111 + i, c.y[i] = 14;
    pre_defined_corridors[1][2] = pre_defined_corridors[2][1] = c;

    c.sz = 10;
    for (int i = 0; i <= 4; i++) c.x[i] = 111 + i, c.y[i] = 16;
    for (int i = 5; i <= 9; i++) c.x[i] = 115, c.y[i] = 17 + i - 5;
    pre_defined_corridors[1][4] = pre_defined_corridors[4][1] = c;

    c.sz = 21;
    for (int i = 0; i <= 10; i++) c.x[i] = 146 + i, c.y[i] = 14;
    for (int i = 11; i <= 20; i++) c.x[i] = 156, c.y[i] = 15 + i - 12;
    pre_defined_corridors[2][5] = pre_defined_corridors[5][2] = c;

    c.sz = 30;
    for (int i = 0; i <= 19; i++) c.x[i] = 141, c.y[i] = 19 + i;
    for (int i = 20; i <= 29; i++) c.x[i] = 141 - i + 19, c.y[i] = 38;
    pre_defined_corridors[2][7] = pre_defined_corridors[7][2] = c;

    c.sz = 15;
    for (int i = 0; i <= 14; i++) c.x[i] = 85 + i, c.y[i] = 25;
    pre_defined_corridors[3][4] = pre_defined_corridors[4][3] = c;

    c.sz = 4;
    for (int i = 0; i <= 3; i++) c.x[i] = 78, c.y[i] = 29 + i;
    pre_defined_corridors[3][6] = pre_defined_corridors[6][3] = c;

    c.sz = 24;
    for (int i = 0; i <= 24; i++) c.x[i] = 126 + i, c.y[i] = 28;
    pre_defined_corridors[4][5] = pre_defined_corridors[5][4] = c;

    c.sz = 4;
    for (int i = 0; i <= 3; i++) c.x[i] = 121, c.y[i] = 31 + i;
    pre_defined_corridors[4][7] = pre_defined_corridors[7][4] = c;

    c.sz = 131;
    for (int i = 0; i <= 17; i++) c.x[i] = 159, c.y[i] = 24 - i - 1;
    for (int i = 18; i <= 130; i++) c.x[i] = 159 - i + 17, c.y[i] = 6;
    pre_defined_corridors[5][8] = pre_defined_corridors[8][5] = c;

    c.sz = 14;
    for (int i = 0; i <= 13; i++) c.x[i] = 96 + i, c.y[i] = 39;
    pre_defined_corridors[6][7] = pre_defined_corridors[7][6] = c;

    c.sz = 51;
    for (int i = 0; i <= 29; i++) c.x[i] = 70 - i - 1, c.y[i] = 37;
    for (int i = 30; i <= 50; i++) c.x[i] = 40, c.y[i] = 37 - i - 1 + 30;
    pre_defined_corridors[6][8] = pre_defined_corridors[8][6] = c;
}
void make_levels();
void print_game();
void draw_game_border();
void print_player();
void print_corridors();
int dist(int x1, int y1, int x2, int y2);
int inroom(int x, int y);
void print_unders();
void resume_game();
void save_game();
void move_player();
int valid(int x, int y);
void update_player_state();
void move_f();
int empty(int x, int y);
void make_secret_doors();


int main(){
    srand(time(NULL));
    make_default_rooms();
    make_default_corridors();
    setlocale(LC_ALL, "");
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
        init_pair(5, COLOR_WHITE, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(8, COLOR_GREEN, COLOR_BLACK);
        init_pair(9, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(10, 46, 46); 
        init_pair(11, 254, 254);
        init_pair(12, 46, COLOR_BLACK); //light green on black
        init_pair(13, 214, COLOR_BLACK); //light Orange on black
    }
    else return 0;
    menus();
    pregame();
    while(1){
        print_game();
        move_player();
        update_player_state();
    }
    close_files();
}














int check_correct_password(char* password){
    rewind(passwords_ptr);
    char s[30];
    int cnt = 0;
    while(fgets(s, 30, passwords_ptr) != NULL){
        s[strlen(s) - 1] = '\0';
        if (cnt == USERNUMBER){
            if (strcmp(s, password) == 0) return 1;
            return 0;
        }
        cnt++;
    }
    return 0;
}
int find_username(char* username){
    rewind(users_ptr);
    char s[30];
    int cnt = 0;
    while(fgets(s, 30, users_ptr) != NULL){
        s[strlen(s) - 1] = '\0';
        if (strcmp(s, username) == 0) return cnt;
        cnt++;
    }
    return 0;
}
void login_menu(){
    clear();
    char username[30];
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
        sleep(1);
        if (check_username(username)){
            clear();
            attron(COLOR_PAIR(4));
            attron(A_BLINK);
            mvprintw(3, COLS / 2 - 15, "Username doesn't exist!");
            attroff(COLOR_PAIR(4));
            attroff(A_BLINK);
            refresh();
            sleep(2);
            clear();
            noecho();
            menus();
            return;
        }
        USERNUMBER = find_username(username);
        while(1){
            clear();
            draw_border_menu();
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(3, COLS / 2 - 20, "Enter Password:");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
            refresh();
            scanw("%s", password);
            if (strcmp(password, "q") == 0){
                noecho();
                clear();
                menus();
            }
            if (!check_correct_password(password)){
                attron(COLOR_PAIR(4));
                attron(A_BLINK);
                mvprintw(3, COLS / 2 - 20, "Wrong Password, Enter q to exit");
                attroff(COLOR_PAIR(4));
                attroff(A_BLINK);
                refresh();
                sleep(1);
            }
            else{
                noecho();
                clear();
                refresh();
                return;
            }
        }
    }
}
int countlines(FILE* file){
    rewind(users_ptr);
    char s[100];
    int ans = 0;
    while(fgets(s, 100, users_ptr) != NULL){
        ans++;
    }
    return ans;
}
void add_user(char *username, char *password, char *email){
    USERNUMBER = countlines(users_ptr);
    fseek(emails_ptr, 0, SEEK_END);
    fseek(passwords_ptr, 0, SEEK_END);
    fseek(scores_ptr, 0, SEEK_END);
    fseek(users_ptr, 0, SEEK_END);
    fseek(golds_ptr, 0, SEEK_END);
    fseek(games_ptr, 0, SEEK_END);
    fseek(register_times_ptr, 0, SEEK_END);
    fprintf(users_ptr, "%s\n", username);
    fprintf(passwords_ptr, "%s\n", password);
    fprintf(emails_ptr, "%s\n", email);
    fprintf(scores_ptr, "%d\n", 0);
    fprintf(golds_ptr, "%d\n", 0);
    fprintf(games_ptr, "%d\n", 0);
    time_t t;
    time(&t);
    fprintf(register_times_ptr, "%ld\n", t);
    close_files();
    open_files();
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
void draw_border_scoreboard(){
    attron(COLOR_PAIR(1));
    for (int i = 0; i <= 30; i++){
        mvprintw(i, COLS / 2 - 50, " ");
        mvprintw(i, COLS / 2 + 50," ");
    }
    for (int i = COLS / 2 - 50; i <= COLS / 2 + 50; i++){
        mvprintw(0, i," ");
        mvprintw(30, i," ");
    }
    refresh();
    attroff(COLOR_PAIR(1));
}
int check_username(char *username){
    rewind(users_ptr);
    char s[30];
    while(fgets(s, 30, users_ptr) != NULL){
        s[strlen(s) - 1] = '\0';
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
                mvprintw(3, COLS / 2 - 15, "Password should be longer than 7 characters");
                mvprintw(4, COLS / 2 - 15, "and should contain at least one number,");
                mvprintw(5, COLS / 2 - 15, "lowercase letter and one uppercase letter");
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
    add_user(username, password, email);
    clear();
    menus();
}
void menus(){
    clear();
    int curmenu = 0;
    int total_menus = 4;
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
            mvprintw(9, (COLS - 10) / 2, "Exit");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(9, (COLS - 10) / 2, "Exit");
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
                login_menu();
                return;
            }
            else if (curmenu == 2){
                USERNUMBER = -1;
                clear();
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
    users_ptr = fopen("users.txt", "a+");
    scores_ptr = fopen("scores.txt", "a+");
    passwords_ptr = fopen("passwords.txt", "a+");
    emails_ptr = fopen("emails.txt", "a+");
    golds_ptr = fopen("golds.txt", "a+");
    games_ptr = fopen("gmaes.txt", "a+");
    register_times_ptr = fopen("register_times.txt", "a+");
}
void close_files(){
    fclose(users_ptr);
    fclose(scores_ptr);
    fclose(passwords_ptr);
    fclose(emails_ptr);
    fclose(golds_ptr);
    fclose(games_ptr);
    fclose(register_times_ptr);
}
void scoreboard(){
    clear();
    for (int i = 0; i < 1000; i++) mark[i] = 0;
    int cnt = 0;
    draw_border_scoreboard();
    attron(COLOR_PAIR(2));
    attron(A_BOLD);
    mvprintw(1, COLS / 2 - 7, "SCOREBOARD");
    attroff(A_BOLD);
    mvprintw(2, COLS / 2 - 12, "press any key to continue");
    attroff(COLOR_PAIR(2));
    for (int i = 0; i < 10; i++){
        printmx(cnt, cnt % 10 + 5);
        cnt++;
    }
    refresh();
    getch();
    clear();
    pregame();
}
void printmx(int cnt, int line){
    int mx = -1;
    int total = countlines(users_ptr);
    if (cnt >= total) return;
    rewind(scores_ptr); rewind(users_ptr); rewind(golds_ptr); rewind(games_ptr); rewind(register_times_ptr);
    int score = 0, golds = 0, games = 0;
    int scorex = 0, goldsx = 0, gamesx = 0, ind = 0;
    time_t tx = 0;
    time_t t = 0;
    time_t cur_t;
    time(&cur_t);
    char user[30], userx[30];
    for (int i = 0; i < total; i++){
        fgets(user, 30, users_ptr);
        user[strlen(user) - 1] = '\0';
        fscanf(scores_ptr, "%d\n", &score);
        fscanf(golds_ptr, "%d\n", &golds);
        fscanf(games_ptr, "%d\n", &games);
        fscanf(scores_ptr, "%d\n", &score);
        fscanf(register_times_ptr, "%ld\n", &t);
        if (!mark[i] && score > mx){
            mx = score;
            scorex = score, goldsx = golds, gamesx = games, tx = t;
            strcpy(userx, user);
            ind = i;
        }
    }
    t = difftime(cur_t, tx);
    if (ind == USERNUMBER){
        attron(A_BOLD);
    }
    if (cnt == 0){
        mvprintw(line, COLS / 2 - 48, "GOAT 🥇");
        attron(COLOR_PAIR(6));
        mvprintw(line, COLS / 2 - 41, "%d: %s Score: %d Golds: %d Games: %d Time since registered: %ld days %ld hours %ld minutes", cnt + 1, userx, scorex, goldsx, gamesx, 
        t / 86400, (t % 86400) / 3600 , ((t % 86400) % 3600) / 60);
        attroff(COLOR_PAIR(6));
    }
    else if (cnt == 1){
        mvprintw(line, COLS / 2 - 43, "🥈");
        attron(COLOR_PAIR(6));
        mvprintw(line, COLS / 2 - 41, "%d: %s Score: %d Golds: %d Games: %d Time since registered: %ld days %ld hours %ld minutes", cnt + 1, userx, scorex, goldsx, gamesx, 
        t / 86400, (t % 86400) / 3600 , ((t % 86400) % 3600) / 60);
        attroff(COLOR_PAIR(6));
    }
    else if (cnt == 2){
        mvprintw(line, COLS / 2 - 43, "🥉");
        attron(COLOR_PAIR(6));
        mvprintw(line, COLS / 2 - 41, "%d: %s Score: %d Golds: %d Games: %d Time since registered: %ld days %ld hours %ld minutes", cnt + 1, userx, scorex, goldsx, gamesx, 
        t / 86400, (t % 86400) / 3600 , ((t % 86400) % 3600) / 60);
        attroff(COLOR_PAIR(6));
    }
    else{
        attron(COLOR_PAIR(5));
        mvprintw(line, COLS / 2 - 41, "%d: %s Score: %d Golds: %d Games: %d Time since registered: %ld days %ld hours %ld minutes", cnt + 1, userx, scorex, goldsx, gamesx, 
        t / 86400, (t % 86400) / 3600 , ((t % 86400) % 3600) / 60);
        attroff(COLOR_PAIR(5));
    }
    if (ind == USERNUMBER){
        attroff(A_BOLD);
    }
}
void pregame(){
    clear();
    int curmenu = 0;
    int total_menus = 6;
    while(1){
        clear();
        attron(A_BOLD);
        attron(A_BLINK);
        attron(COLOR_PAIR(2));
        mvprintw(4, (COLS - 5) / 2, "Game Menu");
        attroff(A_BOLD);
        attroff(A_BLINK);
        attroff(COLOR_PAIR(2));
        draw_border_menu();
        if (curmenu == 0){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(6, (COLS - 10) / 2, "New Game");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(6, (COLS - 10) / 2, "New Game");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 1){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(7, (COLS - 10) / 2, "Resume Game");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(7, (COLS - 10) / 2, "Resume Game");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 2){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(8, (COLS - 10) / 2, "Scoreboard");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(8, (COLS - 10) / 2, "Scoreboard");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 3){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(9, (COLS - 10) / 2, "Settings");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(9, (COLS - 10) / 2, "Settings");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 4){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(10, (COLS - 10) / 2, "Profile");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(10, (COLS - 10) / 2, "Profile");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 5){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(11, (COLS - 10) / 2, "Exit");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(11, (COLS - 10) / 2, "Exit");
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
                new_game();
                return;
            }
            else if (curmenu == 1){
                resume_game();
                return;
            }
            else if (curmenu == 2){
                clear();
                scoreboard();
                return;
            }
            else if (curmenu == 3){
                clear();
                // settings();
                return;
            }
            else if (curmenu == 4){
                clear();
                profile();
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
void profile(){
    clear();
    draw_border_menu();
    if (USERNUMBER == -1){
        attron(COLOR_PAIR(4));
        mvprintw(5, COLS / 2 - 20, "GUEST USER! PLEASE LOGIN TO VIEW PROFILE");
        refresh();
        sleep(2);
        attroff(COLOR_PAIR(4));
        menus();
        return;
    }
    attron(COLOR_PAIR(2));
    mvprintw(2, COLS / 2 - 12, "press any key to continue");
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(5));
    attron(A_BOLD);
    rewind(users_ptr);
    char s[30];
    for (int i = 0; i <= USERNUMBER; i++){
        fgets(s, 30, users_ptr);
    }
    mvprintw(5, COLS / 2 - 10, "USER: %s", s);
    rewind(scores_ptr);
    int num = 0;
    for (int i = 0; i <= USERNUMBER; i++){
        fscanf(scores_ptr, "%d", &num);
    }
    mvprintw(6, COLS / 2 - 10, "SCORE: %d", num);
    rewind(golds_ptr);
    num = 0;
    for (int i = 0; i <= USERNUMBER; i++){
        fscanf(golds_ptr, "%d", &num);
    }
    mvprintw(7, COLS / 2 - 10, "GOLDS: %d", num);
    rewind(games_ptr);
    num = 0;
    for (int i = 0; i <= USERNUMBER; i++){
        fscanf(games_ptr, "%d", &num);
    }
    mvprintw(8, COLS / 2 - 10, "GAMES PLAYED: %d", num);
    attroff(COLOR_PAIR(5));
    attroff(A_BOLD);
    refresh();
    getch();
    clear();
    pregame();
}
void make_levels(){
    for (int le = 0; le < 5; le++){
        level_struct l;
        l.number_of_rooms = 6;
        int mark[9];
        for (int i = 0; i < 9; i++) mark[i] = 0;
        int first_room = -1;
        for (int i = 0; i < l.number_of_rooms; i++){
            int x = rand() % 9;
            while(mark[x]){
                x = rand() % 9;
            }
            mark[x] = 1;
            if (le == 0 && first_room == -1){
                first_room = x;
                l.rooms[i] = pre_defined_rooms[x];
                l.rooms[i].vis = 1;
                l.room_numbers[i] = x;
                game.player.x = l.rooms[i].x1 + 3, game.player.y = l.rooms[i].y1 + 3;
            }
            else l.rooms[i] = pre_defined_rooms[x], l.room_numbers[i] = x;
        }
        if (le == 4) continue;
        int x = rand() % 6;
        l.stairs_x = l.rooms[x].x1 + 1;
        l.stairs_y = l.rooms[x].y1 + 2;
        l.rooms[x].in[1][2] = 1;
        game.levels[le] = l;
        
    }
}
void make_secret_doors(){
    for (int le = 0; le < 5; le++){
        level_struct l = game.levels[le];
        int mark[9];
        for (int i = 0; i < 9; i++) mark[i] = 0;
        for (int i = 0; i < l.number_of_rooms; i++){
            mark[l.room_numbers[i]] = 1;
        }
        for (int i = 0; i < l.number_of_rooms; i++){
            int cnt = 0;
            for (int j = 0; j < l.rooms[i].number_of_doors; j++){
                if (mark[l.rooms[i].other_room[j]]){
                   cnt++;
                }
            }
            if (cnt >= 1){
                for (int j = 0; j < l.rooms[i].number_of_doors; j++){
                    if (mark[l.rooms[i].other_room[j]]){
                        int x = rand() % 5;
                        if (x == 0){
                            game.levels[le].rooms[i].secret_door[j] = 1;
                        }
                    }
                }
            }
        }
    }
}
void new_game(){
    make_levels();
    make_secret_doors();
    game.golds = 0;
    game.score = 0;
    game.level = 0;
    game.health = 0;
    game.energy = 20;
    for (int i = 0; i < 300; i++){
        for (int j = 0; j < 300; j++){
            for (int l = 0; l < 10; l++) game.vis_corridors[l][i][j] = 0;
        }
    }
}
void resume_game(){
    char filename[100];
    snprintf(filename, sizeof(filename), "%d.save", USERNUMBER);
    FILE *file = fopen(filename, "rb");
    size_t result = fread(&game, sizeof(game_struct), 1, file);
    fclose(file);
}
void save_game() {
    char filename[100];
    snprintf(filename, sizeof(filename), "%d.save", USERNUMBER);
    FILE *file = fopen(filename, "wb");
    size_t result = fwrite(&game, sizeof(game_struct), 1, file);
    fclose(file);
}
void draw_game_border(){
    attron(COLOR_PAIR(2));
    for (int i = 0; i <= 47; i++){
        mvprintw(i, 20, ".");
        mvprintw(i, 170, ".");
    }
    for (int i = 20; i <= 170; i++){
        mvprintw(0, i,".");
        mvprintw(47, i,".");
    }
    refresh();
    attroff(COLOR_PAIR(2));
}
int inroom(int x, int y){
    level_struct l = game.levels[game.level];
    for (int i = 0; i < l.number_of_rooms; i++){
        if (l.rooms[i].x1 <= x && x <= l.rooms[i].x2 && l.rooms[i].y1 <= y && y <= l.rooms[i].y2) return 1;
    }
    return 0;
}
void print_rooms(){
    int cnt = game.level;
    level_struct l = game.levels[cnt];
    cnt = l.number_of_rooms;
    int mark[9];
    for (int i = 0; i < 9; i++) mark[i] = 0;
    for (int i = 0; i < l.number_of_rooms; i++){
        mark[l.room_numbers[i]] = 1;
    }
    for (int i = 0; i < cnt; i++){
        if (!l.rooms[i].vis) continue;
        int x1 = l.rooms[i].x1, y1 = l.rooms[i].y1;
        int x2 = l.rooms[i].x2, y2 = l.rooms[i].y2;

        attron(A_BOLD);
        attron(COLOR_PAIR(4));
        for (int x = x1; x <= x2; x++){
            mvprintw(y1, x, "=");
            mvprintw(y2, x, "=");
        }
        for (int y = y1; y <= y2; y++){
            mvprintw(y, x1, "|");
            mvprintw(y, x2, "|");
        }
        attroff(COLOR_PAIR(4));
        for (int j = 0; j < l.rooms[i].number_of_doors; j++){
            attron(COLOR_PAIR(2));
            if (mark[l.rooms[i].other_room[j]] && !l.rooms[i].secret_door[j]){
                mvprintw(l.rooms[i].doors_y[j], l.rooms[i].doors_x[j], "+");
            }
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(13));
            if (l.rooms[i].secret_door[j] && dist(game.player.x, game.player.y, l.rooms[i].doors_x[j], l.rooms[i].doors_y[j]) <= 1){
                mvprintw(l.rooms[i].doors_y[j], l.rooms[i].doors_x[j], "+");
            }
            attroff(COLOR_PAIR(13));
        }
        attroff(A_BOLD);
        for (int x = x1 + 1; x < x2; x++){
            for (int y = y1 + 1; y < y2; y++){
                if (l.rooms[i].in[x - x1][y - y1] == 1){
                    attron(COLOR_PAIR(9));
                    mvprintw(y, x, "S");
                    attroff(COLOR_PAIR(9));
                }
                else{
                    attron(COLOR_PAIR(8));
                    mvprintw(y, x, ".");
                    attroff(COLOR_PAIR(8));
                }
            }
        }
    }
    refresh();
}
void print_player(){
    mvprintw(game.player.y, game.player.x, "👾");
    refresh();
}
int dist(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}
void print_corridors(){
    level_struct l = game.levels[game.level];
    int mark[9];
    for (int i = 0; i < 9; i++) mark[i] = 0;
    for (int i = 0; i < l.number_of_rooms; i++){
        mark[l.room_numbers[i]] = 1;
    }
    for (int i = 0; i < 9; i++){
        for (int j = i + 1; j < 9; j++){
            if (mark[i] && mark[j] && pre_defined_corridors[i][j].sz){
                for (int k = 0; k < pre_defined_corridors[i][j].sz; k++){
                    int x = pre_defined_corridors[i][j].x[k], y = pre_defined_corridors[i][j].y[k];
                    if (game.vis_corridors[game.level][x][y] || (!inroom(game.player.x, game.player.y) && dist(game.player.x, game.player.y, x, y) <= 4)){
                        mvprintw(y, x, "#");
                    }
                }
            }
        }
    }
}
void print_game(){
    clear();
    draw_game_border();
    print_rooms();
    print_corridors();
    print_player();
    print_unders();
    refresh();
}
void print_unders(){
    attron(COLOR_PAIR(2));
    attron(A_BOLD);
    mvprintw(45, 27, "LEVEL:%d  GOLDS:%d  HEALTH:%d  ENERGY:", game.level + 1, game.golds, game.health);
    attroff(COLOR_PAIR(2));
    attroff(A_BOLD);
    int x_enery = 63;
    for (int i = 0; i < game.energy; i++){
        attron(COLOR_PAIR(10) | A_DIM);
        mvprintw(45, x_enery++, "˩");
        attroff(COLOR_PAIR(10) | A_DIM);
    }
    for (int i = game.energy; i < 20; i++){
        attron(COLOR_PAIR(11) | A_DIM);
        mvprintw(45, x_enery++, "˩");
        attroff(COLOR_PAIR(11) | A_DIM);
    }
}
int valid(int x, int y){
    level_struct l = game.levels[game.level];
    int inroom = 0;
    for (int i = 0; i < l.number_of_rooms; i++){
        bool check_door = 0;
        for (int j = 0; j < l.rooms[i].number_of_doors; j++){
            if (x == l.rooms[i].doors_x[j] && y == l.rooms[i].doors_y[j]) check_door = 1;
        }
        if (check_door) return 1;
        if ((x == l.rooms[i].x1 || x == l.rooms[i].x2) && l.rooms[i].y1 <= y && y <= l.rooms[i].y2) return 0;
        if ((y == l.rooms[i].y1 || y == l.rooms[i].y2) && l.rooms[i].x1 <= x && x <= l.rooms[i].x2) return 0;
        if (l.rooms[i].y1 <= y && y <= l.rooms[i].y2 && l.rooms[i].x1 <= x && x <= l.rooms[i].x2) inroom = 1;
    }
    if (inroom) return 1;
    int mark[9];
    for (int i = 0; i < 9; i++) mark[i] = 0;
    for (int i = 0; i < l.number_of_rooms; i++){
        mark[l.room_numbers[i]] = 1;
    }
    for (int i = 0; i < 9; i++){
        for (int j = i + 1; j < 9; j++){
            if (mark[i] && mark[j] && pre_defined_corridors[i][j].sz){
                for (int k = 0; k < pre_defined_corridors[i][j].sz; k++){
                    int xc = pre_defined_corridors[i][j].x[k], yc = pre_defined_corridors[i][j].y[k];
                    if (x == xc && y == yc) return 1;
                }
            }
        }
    }
    return 0;
}
int empty(int x, int y){
    level_struct l = game.levels[game.level];
    int inroom = 0;
    for (int i = 0; i < l.number_of_rooms; i++){
        bool check_door = 0;
        for (int j = 0; j < l.rooms[i].number_of_doors; j++){
            if (x == l.rooms[i].doors_x[j] && y == l.rooms[i].doors_y[j]) check_door = 1;
        }
        if (check_door) return 0;
        if (l.rooms[i].y1 <= y && y <= l.rooms[i].y2 && l.rooms[i].x1 <= x && x <= l.rooms[i].x2) inroom = 1;
    }
    if (!inroom) return 0;
    for (int i = 0; i < l.number_of_rooms; i++){
        for (int xc = l.rooms[i].x1 + 1; xc < l.rooms[i].x2; xc++){
            for (int yc = l.rooms[i].y1 + 1; yc < l.rooms[i].y2; yc++){
                if (x == xc && y == yc){
                    if (l.rooms[i].in[xc - l.rooms[i].x1][yc - l.rooms[i].y1] == 0) return 1;
                    else return 0;
                }
            }
        }
    }
    return 0;
}
void move_f(){
    char ch = getch();
    if (ch == 'y'){
        while(valid(game.player.x - 1, game.player.y - 1) && empty(game.player.x - 1, game.player.y - 1)){
            game.player.x--, game.player.y--;
        }
        if (valid(game.player.x - 1, game.player.y - 1)){
            game.player.x--, game.player.y--;
        }
    }
    else if (ch == 'u'){
        while(valid(game.player.x + 1, game.player.y - 1) && empty(game.player.x + 1, game.player.y - 1)){
            game.player.x++, game.player.y--;
        }
        if (valid(game.player.x + 1, game.player.y - 1)){
            game.player.x++, game.player.y--;
        }
    }
    else if (ch == 'b'){
        while(valid(game.player.x - 1, game.player.y + 1) && empty(game.player.x - 1, game.player.y + 1)){
            game.player.x--, game.player.y++;
        }
        if (valid(game.player.x - 1, game.player.y + 1)){
            game.player.x--, game.player.y++;
        }
    }
    else if (ch == 'n'){
        while(valid(game.player.x + 1, game.player.y + 1) && empty(game.player.x + 1, game.player.y + 1)){
            game.player.x++, game.player.y++;
        }
        if (valid(game.player.x + 1, game.player.y + 1)){
            game.player.x++, game.player.y++;
        }
    }
    else if (ch == 'h'){
        while(valid(game.player.x - 1, game.player.y) && empty(game.player.x - 1, game.player.y)){
            game.player.x--;
        }
        if (valid(game.player.x - 1, game.player.y)){
            game.player.x--;
        }
    }
    else if (ch == 'j'){
        while(valid(game.player.x, game.player.y - 1) && empty(game.player.x, game.player.y - 1)){
            game.player.y--;
        }
        if (valid(game.player.x, game.player.y - 1)){
            game.player.y--;
        }
    }
    else if (ch == 'k'){
        while(valid(game.player.x, game.player.y + 1) && empty(game.player.x, game.player.y + 1)){
            game.player.y++;
        }
        if (valid(game.player.x, game.player.y + 1)){
            game.player.y++;
        }
    }
    else if (ch == 'l'){
        while(valid(game.player.x + 1, game.player.y) && empty(game.player.x + 1, game.player.y)){
            game.player.x++;
        }
        if (valid(game.player.x + 1, game.player.y)){
            game.player.x++;
        }
    }
}
void move_player(){
    noecho();
    char ch = getch();
    if (ch == 'y' && valid(game.player.x - 1, game.player.y - 1)){
        game.player.x--, game.player.y--;
    }
    else if (ch == 'u' && valid(game.player.x + 1, game.player.y - 1)){
        game.player.x++, game.player.y--;
    }
    else if (ch == 'b' && valid(game.player.x - 1, game.player.y + 1)){
        game.player.x--, game.player.y++;
    }
    else if (ch == 'n' && valid(game.player.x + 1, game.player.y + 1)){
        game.player.x++, game.player.y++;
    }
    else if (ch == 'h' && valid(game.player.x - 1, game.player.y)){
        game.player.x--;
    }
    else if (ch == 'j' && valid(game.player.x, game.player.y - 1)){
        game.player.y--;
    }
    else if (ch == 'k' && valid(game.player.x, game.player.y + 1)){
        game.player.y++;
    }
    else if (ch == 'l' && valid(game.player.x + 1, game.player.y)){
        game.player.x++;
    }
    else if (ch == 'f'){
        move_f();
    }
    else if (ch == '>' && game.player.x == game.levels[game.level].stairs_x && game.player.y == game.levels[game.level].stairs_y){
        game.level++;
        attron(COLOR_PAIR(12));
        attron(A_BOLD);
        mvprintw(2, 27, "YOU HAVE ENTERED A NEW LEVEL");
        refresh();
        sleep(2);
        attroff(COLOR_PAIR(12));
        attroff(A_BOLD);
        return;
    }
    // else if (ch == '<' && game.player.x == game.levels[game.level].stairs_x && game.player.y == game.levels[game.level].stairs_y && game.level > 0){
    //     game.level--;
    //     return;
    // }
    else{
        move_player();
    }
}
void update_player_state(){
    int x = game.player.x, y = game.player.y;
    game.vis_corridors[game.level][x][y] = 1;
    //new room
    level_struct l = game.levels[game.level];
    for (int i = 0; i < l.number_of_rooms; i++){
        if (l.rooms[i].y1 <= y && y <= l.rooms[i].y2 && l.rooms[i].x1 <= x && x <= l.rooms[i].x2){
            if (!game.levels[game.level].rooms[i].vis){
                attron(COLOR_PAIR(12));
                attron(A_BOLD);
                mvprintw(2, 27, "YOU HAVE ENTERED A NEW ROOM");
                refresh();
                sleep(2);
                attroff(COLOR_PAIR(12));
                attroff(A_BOLD);
            }
            game.levels[game.level].rooms[i].vis = 1;
        }
    }
}