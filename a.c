#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ncurses.h>


FILE *users_ptr;
int mark[1000];

typedef struct{
    int x;
    int y;
    int health;
    int damage;
    int alive;
    int len;
    char name;
    int touch;
    int lensave;
}monster_struct;
typedef struct{
    int number_of_foods;
    int foods[20];
    int weapon_cnt[20];
    int potion_cnt[20];
}inventory_struct;
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
    int in[50][50];
    int secret_door[10];
    int password_door[10];
    int open_password_door[10];
    int password;
    monster_struct monster;
    monster_struct last_level_monsters[300];
    int used_weapon[30][30];
    //0 = empty 1 = stairs 2 = password generator 3 = trap 4 = trasure 5 = normal food 6 = normal gold 7 = black gold 8 = Mace 9 = Dagger 10 = Magic wand
    //11 = normal arrow 12 = sword 13 = Health 14 = Speed 15 = Damage
}room_struct;
typedef struct{
    int number_of_rooms;
    room_struct rooms[10];
    int room_numbers[10];
    int stairs_x;
    int stairs_y;
    int treasure_room[10];
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
    int difficulty;
    int m;
    int moves;
    int cur_weapon;
    inventory_struct inventory;
    int time_since_last_attack;
    int time_since_last_health_potion;
    int time_since_last_speed_potion;
    int time_since_last_damage_potion;
    int player_color;
}game_struct;
typedef struct{
    int guest;
    char email[30];
    char password[30];
    char username[30];
    int score;
    int gold;
    int games;
    time_t register_time;
}user_struct;



room_struct pre_defined_rooms[10];
corridor_struct pre_defined_corridors[10][10];
game_struct game;
user_struct user;
int weapon_damage[20];
char* weapon_name[20];
int weapon_dist[20];
int weapon_num[20];
int move_g = 0;
char last_fight;


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
    room1.number_of_doors = 4;
    room1.doors_x[0] = 58; room1.doors_x[1] = 69; room1.doors_x[2] = 75; room1.doors_x[3] = 75;
    room1.doors_y[0] = 11; room1.doors_y[1] = 16; room1.doors_y[2] = 15; room1.doors_y[3] = 9;
    room1.other_room[0] = 8, room1.other_room[1] = 3, room1.other_room[2] = 1, room1.other_room[3] = 6;
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
    room6.number_of_doors = 4;
    room6.doors_x[0] = 150; room6.doors_x[1] = 156; room6.doors_x[2] = 159; room6.doors_x[3] = 159;
    room6.doors_y[0] = 28; room6.doors_y[1] = 24; room6.doors_y[2] = 24; room6.doors_y[3] = 24;
    room6.other_room[0] = 4, room6.other_room[1] = 2, room6.other_room[2] = 8, room6.other_room[3] = 0;
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
        for (int x = 0; x < 50; x++){
            for (int y = 0; y < 50; y++){
                pre_defined_rooms[i].in[x][y] = 0;
            }
        }
    }
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 10; j++){
            pre_defined_rooms[i].secret_door[j] = 0;
            pre_defined_rooms[i].password_door[j] = 0;
            pre_defined_rooms[i].open_password_door[j] = 0;
            pre_defined_rooms[i].password = 0;
        }
        for (int j = 0; j < 30; j++){
            for (int k = 0; k < 30; k++){
                pre_defined_rooms[i].used_weapon[j][k] = 0;
            }
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

    c.sz = 98;
    for (int i = 0; i <= 83; i++) c.x[i] = 75 + i + 1, c.y[i] = 9;
    for (int i = 84; i <= 97; i++) c.x[i] = 159, c.y[i] = 9 + 1 + i - 84;
    pre_defined_corridors[0][5] = pre_defined_corridors[5][0] = c;
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
void settings();
void difficulty_settings();
void win();
void save_user();
void inventory_menu();
void draw_inventory_border();
void lose();
void weapon_menu();
void print_monsters();
void move_monster(int mx, int my);
void pick_weapon(int ind);
void fight();
int monster_damage(int x, int y);
void shoot_line(int xx, int yy, int d);
int is_wall(int x, int y);
void put_weapon(int x, int y);
void fight_a();
void potion_menu();
void pick_potion(int ind);
void character_settings();






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
    // mvprintw(1, 1, "✬.");
    // mvprintw(2, 1, "..");
    // refresh();
    // sleep(3);
    weapon_damage[8] = 5, weapon_damage[9] = 12, weapon_damage[10] = 15, weapon_damage[11] = 5, weapon_damage[12] = 10;
    weapon_name[8] = "MACE", weapon_name[9] = "DAGGER", weapon_name[10] = "MAGIC_WAND", weapon_name[11] = "NORMAL_ARROW", weapon_name[12] = "SWORD";
    weapon_dist[8] = 1, weapon_dist[9] = 5, weapon_dist[10] = 10, weapon_dist[11] = 5, weapon_dist[12] = 1;
    weapon_num[8] = 1, weapon_num[9] = 10, weapon_num[10] = 8, weapon_num[11] = 20, weapon_num[12] = 1000;
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
        init_pair(14, 81, COLOR_BLACK); //light cyan
        init_pair(15, 214, COLOR_YELLOW); //light orange on yellow
        init_pair(16, COLOR_RED, COLOR_YELLOW);
        init_pair(17, COLOR_BLACK, COLOR_BLACK);
        init_pair(18, 130, COLOR_BLACK);//brown
        init_pair(19, 235, COLOR_BLACK);//dark gray
        init_pair(20, 130, 240); //brown on grey
        init_pair(21, COLOR_WHITE, 240);
        init_pair(22, COLOR_CYAN, COLOR_BLACK);
        init_pair(23, 213, COLOR_BLACK); //light pink on black
        init_pair(24, 34, COLOR_BLACK); //health
        init_pair(25, 13, COLOR_BLACK); //speed
        init_pair(26, 214, COLOR_BLACK); // damage
        init_pair(27, COLOR_CYAN, COLOR_BLACK);
        init_pair(28, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(29, COLOR_RED, COLOR_BLACK);
        init_pair(30, COLOR_CYAN, COLOR_WHITE);
        init_pair(31, COLOR_MAGENTA, COLOR_WHITE);
        init_pair(32, COLOR_RED, COLOR_WHITE);
    }
    else return 0;
    game.difficulty = 1;
    game.player_color = 27;
    menus();
    pregame();
    while(game.health){
        print_game();
        move_player();
        update_player_state();
    }
    lose();
    close_files();
}






int check_correct_password(char* password){
    char s[30];
    if (strcmp(password, user.password) == 0) return 1;
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
        char filename[100];
        snprintf(filename, sizeof(filename), "%s.save", username);
        FILE *file = fopen(filename, "rb");
        size_t result = fread(&user, sizeof(user_struct), 1, file);
        fclose(file);
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
    fseek(users_ptr, 0, SEEK_END);
    fprintf(users_ptr, "%s\n", username);
    strcpy(user.username, username);
    strcpy(user.password, password);
    strcpy(user.email, email);
    user.gold = 0; user.score = 0; user.games = 0;
    time_t t;
    time(&t);
    user.register_time = t;
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.save", user.username);
    FILE *file = fopen(filename, "wb");
    size_t result = fwrite(&user, sizeof(user_struct), 1, file);
    fclose(file);
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
                user.guest = 1;
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
}
void close_files(){
    fclose(users_ptr);
}
void difficulty_settings(){
    clear();
    int curmenu = 0;
    int total_menus = 4;
    while(1){
        clear();
        attron(A_BOLD);
        attron(COLOR_PAIR(2));
        mvprintw(4, (COLS - 16) / 2, "Difficulty Settings");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(2));
        draw_border_menu();
        if (curmenu == 0){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(6, (COLS - 16) / 2, "Easy");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(6, (COLS - 16) / 2, "Easy");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 1){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(7, (COLS - 16) / 2, "Medium");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(7, (COLS - 16) / 2, "Medium");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 2){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(8, (COLS - 16) / 2, "Hard");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(8, (COLS - 16) / 2, "Hard");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 3){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(9, (COLS - 16) / 2, "Exit");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(9, (COLS - 16) / 2, "Exit");
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
                game.difficulty = 0;
                pregame();
                return;
            }
            else if (curmenu == 1){
                game.difficulty = 1;
                settings();
                return;
            }
            else if (curmenu == 2){
                clear();
                game.difficulty = 2;
                settings();
                return;
            }
            else{
                settings();
                return;
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
void character_settings(){
    clear();
    int curmenu = 0;
    int total_menus = 4;
    while(1){
        clear();
        attron(A_BOLD);
        attron(COLOR_PAIR(2));
        mvprintw(4, (COLS - 16) / 2, "Character Settings");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(2));
        draw_border_menu();
        if (curmenu != 0){
            attron(COLOR_PAIR(27));
            attron(A_BLINK);
            mvprintw(6, (COLS - 16) / 2, "P");
            attroff(COLOR_PAIR(27));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(30));
            attron(A_BLINK);
            mvprintw(6, (COLS - 16) / 2, "P");
            attroff(COLOR_PAIR(30));
            attroff(A_BLINK);
        }
        if (curmenu != 1){
            attron(COLOR_PAIR(28));
            attron(A_BLINK);
            mvprintw(7, (COLS - 16) / 2, "P");
            attroff(COLOR_PAIR(28));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(31));
            attron(A_BLINK);
            mvprintw(7, (COLS - 16) / 2, "P");
            attroff(COLOR_PAIR(31));
            attroff(A_BLINK);
        }
        if (curmenu != 2){
            attron(COLOR_PAIR(29));
            attron(A_BLINK);
            mvprintw(8, (COLS - 16) / 2, "P");
            attroff(COLOR_PAIR(29));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(32));
            attron(A_BLINK);
            mvprintw(8, (COLS - 16) / 2, "P");
            attroff(COLOR_PAIR(32));
            attroff(A_BLINK);
        }
        if (curmenu == 3){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(9, (COLS - 16) / 2, "Exit");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(9, (COLS - 16) / 2, "Exit");
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
                game.difficulty = 0;
                pregame();
                return;
            }
            else if (curmenu == 1){
                game.difficulty = 1;
                settings();
                return;
            }
            else if (curmenu == 2){
                clear();
                game.difficulty = 2;
                settings();
                return;
            }
            else{
                settings();
                return;
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
void settings(){
    clear();
    int curmenu = 0;
    int total_menus = 4;
    while(1){
        clear();
        attron(A_BOLD);
        attron(COLOR_PAIR(2));
        mvprintw(4, (COLS - 10) / 2, "Settings");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(2));
        draw_border_menu();
        if (curmenu == 0){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(6, (COLS - 16) / 2, "Difficulty Settings");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(6, (COLS - 16) / 2, "Difficulty Settings");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 1){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(7, (COLS - 16) / 2, "Character Settings");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(7, (COLS - 16) / 2, "Character Settings");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 2){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(8, (COLS - 16) / 2, "Sound settings");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(8, (COLS - 16) / 2, "Sound settings");
            attroff(COLOR_PAIR(2));
            attroff(A_BLINK);
        }
        if (curmenu == 3){
            attron(COLOR_PAIR(3));
            attron(A_BLINK);
            mvprintw(9, (COLS - 16) / 2, "Exit");
            attroff(COLOR_PAIR(3));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(2));
            attron(A_BLINK);
            mvprintw(9, (COLS - 16) / 2, "Exit");
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
                difficulty_settings();
                return;
            }
            else if (curmenu == 1){
                character_settings();
                return;
            }
            else if (curmenu == 2){
                clear();
                // sound_settings();
                return;
            }
            else{
                pregame();
                return;
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
    rewind(users_ptr);
    int scorex = 0, goldsx = 0, gamesx = 0, ind = 0;
    time_t tx = 0;
    time_t t = 0;
    time_t cur_t;
    time(&cur_t);
    char userr[30], userx[30];
    user_struct mxuser;
    for (int i = 0; i < total; i++){
        fgets(userr, 30, users_ptr);
        userr[strlen(userr) - 1] = '\0';
        char filename[100];
        snprintf(filename, sizeof(filename), "%s.save", userr);
        FILE *file = fopen(filename, "rb");
        size_t result = fread(&mxuser, sizeof(user_struct), 1, file);
        fclose(file);
        if (!mark[i] && mxuser.score > mx){
            mx = mxuser.score;
            scorex = mxuser.score, goldsx = mxuser.gold, gamesx = mxuser.games, tx = mxuser.register_time;
            strcpy(userx, userr);
            ind = i;
        }
    }
    mark[ind] = 1;
    t = difftime(cur_t, tx);
    if (strcmp(userx, user.username) == 0){
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
    if (strcmp(userx, user.username) == 0){
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
        mvprintw(4, (COLS - 10) / 2, "Game Menu");
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
                settings();
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
    if (user.guest){
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
    mvprintw(5, COLS / 2 - 10, "USER: %s", user.username);
    mvprintw(6, COLS / 2 - 10, "SCORE: %d", user.score);
    mvprintw(7, COLS / 2 - 10, "GOLDS: %d", user.gold);
    mvprintw(8, COLS / 2 - 10, "GAMES PLAYED: %d", user.games);
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
        l.number_of_rooms = 7;
        for (int i = 0; i < 10; i++) l.treasure_room[i] = 0;
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
        game.levels[le] = l;
    }
    //stairs
    for (int le = 0; le < 5; le++){
        if (le == 4)continue;
        for (int i = 0; i < game.levels[le].number_of_rooms; i++){
            int m = 0;
            for (int j = 0; j < game.levels[le + 1].number_of_rooms; j++){
                if (game.levels[le].room_numbers[i] == game.levels[le + 1].room_numbers[j] && !game.levels[le].rooms[i].vis){
                    game.levels[le].stairs_x = game.levels[le].rooms[i].x1 + 1;
                    game.levels[le].stairs_y = game.levels[le].rooms[i].y1 + 2;
                    game.levels[le].rooms[i].in[1][2] = 1;
                    m = 1;
                    break;
                }
            }
            if (m) break;
        }
    }
    //traps
    for (int le = 0; le < 5; le++){
        if (game.difficulty == 0) break;
        for (int i = 0; i < game.levels[le].number_of_rooms; i++){
            int x = rand() % 2;
            if (x >= 2) continue;
            if (x == 0){
                if (i % 2) game.levels[le].rooms[i].in[4][3] = 3;
                else game.levels[le].rooms[i].in[2][4] = 3;
            }
            else{
                game.levels[le].rooms[i].in[4][3] = 3;
                game.levels[le].rooms[i].in[2][4] = 3;
            }
        }
    }
    //treasure room
    {
        int le = 4;
        int x = rand() % 6;
        game.levels[le].treasure_room[x] = 1;
        game.levels[le].rooms[x].in[5][4] = game.levels[le].rooms[x].in[1][4] = game.levels[le].rooms[x].in[2][4] = game.levels[le].rooms[x].in[4][3] = 3;
        game.levels[le].rooms[x].in[5][5] = 4;
    }
    // normal foods
    {
        for (int le = 0; le < 5; le++){
            for (int i = 0; i < game.levels[le].number_of_rooms; i++){
                int x = rand() % 5;
                if (x == 0) game.levels[le].rooms[i].in[1][1] = 5;
                if (x <= 1) game.levels[le].rooms[i].in[3][2] = 5;
                if (x <= 2) game.levels[le].rooms[i].in[5][3] = 5;
                if (x <= 3) game.levels[le].rooms[i].in[7][5] = 5;
            }
        }
    }
    //golds
    {
        for (int le = 0; le < 5; le++){
            for (int i = 0; i < game.levels[le].number_of_rooms; i++){
                int x = rand() % 10;
                if (x == 0) game.levels[le].rooms[i].in[1][7] = 6;
                if (x <= 1) game.levels[le].rooms[i].in[2][5] = 6;
                if (x <= 2) game.levels[le].rooms[i].in[3][6] = 6;
            }
        }
    }
    //black golds
    {
        for (int le = 0; le < 5; le++){
            for (int i = 0; i < game.levels[le].number_of_rooms; i++){
                int x = rand() % 10;
                if (x == 0) game.levels[le].rooms[i].in[4][1] = 7;
            }
        }
    }
    //weapons
    {
        for (int le = 0; le < 5; le++){
            for (int i = 0; i < game.levels[le].number_of_rooms; i++){
                int x = rand() % 4;
                if (x == 0){
                    int w = rand() % 4 + 1;
                    int p = rand() % 3;
                    if (p == 0) game.levels[le].rooms[i].in[6][1] = 8 + w;
                    else if (p == 1) game.levels[le].rooms[i].in[6][6] = 8 + w;
                    else game.levels[le].rooms[i].in[3][4] = 8 + w;
                }
            }
        }
    }
    //monsters
    {
        for (int le = 0; le < 5; le++){
            for (int i = 0; i < game.levels[le].number_of_rooms; i++){
                int x = rand() % 3;
                if (x == 0){
                    game.levels[le].rooms[i].monster.alive = 1;
                    game.levels[le].rooms[i].monster.touch = 0;
                    int y = rand() % 5;
                    int xr = game.levels[le].rooms[i].x1, yr = game.levels[le].rooms[i].y1;
                    if (y == 0){
                        game.levels[le].rooms[i].monster.x = xr + 2;
                        game.levels[le].rooms[i].monster.y = yr + 2;
                        game.levels[le].rooms[i].monster.health = 5;
                        game.levels[le].rooms[i].monster.damage = 1;
                        game.levels[le].rooms[i].monster.len = 0;
                        game.levels[le].rooms[i].monster.lensave = 0;
                        game.levels[le].rooms[i].monster.name = 'D';
                    }
                    else if (y == 1){
                        game.levels[le].rooms[i].monster.x = xr + 4;
                        game.levels[le].rooms[i].monster.y = yr + 5;
                        game.levels[le].rooms[i].monster.health = 10;
                        game.levels[le].rooms[i].monster.damage = 2;
                        game.levels[le].rooms[i].monster.len = 0;
                        game.levels[le].rooms[i].monster.lensave = 0;
                        game.levels[le].rooms[i].monster.name = 'F';
                    }
                    else if (y == 2){
                        game.levels[le].rooms[i].monster.x = xr + 5;
                        game.levels[le].rooms[i].monster.y = yr + 7;
                        game.levels[le].rooms[i].monster.health = 15;
                        game.levels[le].rooms[i].monster.damage = 3;
                        game.levels[le].rooms[i].monster.len = 5;
                        game.levels[le].rooms[i].monster.lensave = 5;
                        game.levels[le].rooms[i].monster.name = 'G';
                    }
                    else if (y == 3){
                        game.levels[le].rooms[i].monster.x = xr + 5;
                        game.levels[le].rooms[i].monster.y = yr + 2;
                        game.levels[le].rooms[i].monster.health = 20;
                        game.levels[le].rooms[i].monster.damage = 4;
                        game.levels[le].rooms[i].monster.len = 1000;
                        game.levels[le].rooms[i].monster.lensave = 1000;
                        game.levels[le].rooms[i].monster.name = 'S';
                    }
                    else if (y == 4){
                        game.levels[le].rooms[i].monster.x = xr + 7;
                        game.levels[le].rooms[i].monster.y = yr + 3;
                        game.levels[le].rooms[i].monster.health = 30;
                        game.levels[le].rooms[i].monster.damage = 5;
                        game.levels[le].rooms[i].monster.len = 5; 
                        game.levels[le].rooms[i].monster.lensave = 5;
                        game.levels[le].rooms[i].monster.name = 'U';
                    }
                }
                else{
                    game.levels[le].rooms[i].monster.alive = 0;
                }
            }
        }
    }
    //magics
    {
        for (int le = 0; le < 5; le++){
            for (int i = 0; i < game.levels[le].number_of_rooms; i++){
                int x = rand() % 3;
                if (x == 0){
                    game.levels[le].rooms[i].in[7][8] = 13;
                }
                x = rand() % 3;
                if (x == 0){
                    game.levels[le].rooms[i].in[10][6] = 14;
                }
                x = rand() % 3;
                if (x == 0){
                    game.levels[le].rooms[i].in[2][7] = 15;
                }
            }
        }
    }
    {   
    level_struct l;
    l.number_of_rooms = 1;
    l.rooms[0] = pre_defined_rooms[0];
    l.rooms[0].x1 = 70, l.rooms[0].x2 = 120;
    l.rooms[0].y1 = 5, l.rooms[0].y2 = 40;
    l.treasure_room[0] = 1;
    l.rooms[0].number_of_doors = 0; 
    for (int i = 0; i < 300; i++){
        l.rooms[0].last_level_monsters[i].alive = 0;
    }
    l.room_numbers[0] = 0;
    int le = 5;
    game.levels[le] = l;
    for (int i = 0; i < 10; i++){
        int x = rand() % 50 + 70;
        int y = rand() % 35 + 5;
        int z = rand() % 5;
        game.levels[le].rooms[0].last_level_monsters[i].alive = 1;
        if (z == 0){
            game.levels[le].rooms[0].last_level_monsters[i].touch = 0;
            game.levels[le].rooms[0].last_level_monsters[i].x = x;
            game.levels[le].rooms[0].last_level_monsters[i].y = y;
            game.levels[le].rooms[0].last_level_monsters[i].health = 5;
            game.levels[le].rooms[0].last_level_monsters[i].damage = 1;
            game.levels[le].rooms[0].last_level_monsters[i].len = 0;
            game.levels[le].rooms[0].last_level_monsters[i].lensave = 0;
            game.levels[le].rooms[0].last_level_monsters[i].name = 'D';
        }
        else if (z == 1){
            game.levels[le].rooms[0].last_level_monsters[i].x = x;
            game.levels[le].rooms[0].last_level_monsters[i].y = y;
            game.levels[le].rooms[0].last_level_monsters[i].health = 10;
            game.levels[le].rooms[0].last_level_monsters[i].damage = 2;
            game.levels[le].rooms[0].last_level_monsters[i].len = 0;
            game.levels[le].rooms[0].last_level_monsters[i].lensave = 0;
            game.levels[le].rooms[0].last_level_monsters[i].name = 'F';
        }
        else if (z == 2){
            game.levels[le].rooms[0].last_level_monsters[i].x = x;
            game.levels[le].rooms[0].last_level_monsters[i].y = y;
            game.levels[le].rooms[0].last_level_monsters[i].health = 15;
            game.levels[le].rooms[0].last_level_monsters[i].damage = 3;
            game.levels[le].rooms[0].last_level_monsters[i].len = 5;
            game.levels[le].rooms[0].last_level_monsters[i].lensave = 5;
            game.levels[le].rooms[0].last_level_monsters[i].name = 'G';
        }
        else if (z == 3){
            game.levels[le].rooms[0].last_level_monsters[i].x = x;
            game.levels[le].rooms[0].last_level_monsters[i].y = y;
            game.levels[le].rooms[0].last_level_monsters[i].health = 20;
            game.levels[le].rooms[0].last_level_monsters[i].damage = 4;
            game.levels[le].rooms[0].last_level_monsters[i].len = 1000;
            game.levels[le].rooms[0].last_level_monsters[i].lensave = 1000;
            game.levels[le].rooms[0].last_level_monsters[i].name = 'S';
        }
        else if (z == 4){
            game.levels[le].rooms[0].last_level_monsters[i].x = x;
            game.levels[le].rooms[0].last_level_monsters[i].y = y;
            game.levels[le].rooms[0].last_level_monsters[i].health = 30;
            game.levels[le].rooms[0].last_level_monsters[i].damage = 5;
            game.levels[le].rooms[0].last_level_monsters[i].len = 5; 
            game.levels[le].rooms[0].last_level_monsters[i].lensave = 5;
            game.levels[le].rooms[0].last_level_monsters[i].name = 'U';
        }
    }
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
void make_password_doors(){
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
            for (int j = 0; j < l.rooms[i].number_of_doors; j++){
                if (mark[l.rooms[i].other_room[j]]){
                    int x = rand() % 5;
                    if (x == 0){
                        if (!game.levels[le].rooms[i].secret_door[j]){
                            game.levels[le].rooms[i].password_door[j] = 1;
                            int x = game.levels[le].rooms[i].x1, y = game.levels[le].rooms[i].y1;
                            int x2 = game.levels[le].rooms[i].x2, y2 = game.levels[le].rooms[i].y2;
                            game.levels[le].rooms[i].in[x2 - 1 - x][y2 - 1 - y] = 2;
                            break;
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
    make_password_doors();
    game.golds = 0;
    game.moves = 0;
    game.score = 0;
    game.m = 0;
    game.level = 0;
    last_fight = 'a';
    game.health = 100;
    game.energy = 30;
    game.cur_weapon = 8;
    game.time_since_last_attack = 0;
    game.time_since_last_damage_potion = 11;
    game.time_since_last_health_potion = 11;
    game.time_since_last_speed_potion = 11;
    for (int i = 0; i < 20; i++){
        game.inventory.weapon_cnt[i] = 0;
    }
    for (int i = 0; i < 300; i++){
        for (int j = 0; j < 300; j++){
            for (int l = 0; l < 10; l++) game.vis_corridors[l][i][j] = 0;
        }
    }
    game.inventory.number_of_foods = 0;
}
void resume_game(){
    char filename[100];
    snprintf(filename, sizeof(filename), "%sgame.save", user.username);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        pregame();
        return;
    }
    size_t result = fread(&game, sizeof(game_struct), 1, file);
    fclose(file);
}
void save_game() {
    char filename[100];
    snprintf(filename, sizeof(filename), "%sgame.save", user.username);
    FILE *file = fopen(filename, "wb");
    size_t result = fwrite(&game, sizeof(game_struct), 1, file);
    fclose(file);
}
void draw_game_border(){
    attron(COLOR_PAIR(2));
    mvprintw(48, 20, "Press s to save and exit   Press E to open food inventory   Press i to open weapon inventory   Press Y to open potion inventory");
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
        if (!l.rooms[i].vis && !game.m) continue;
        int x1 = l.rooms[i].x1, y1 = l.rooms[i].y1;
        int x2 = l.rooms[i].x2, y2 = l.rooms[i].y2;
        // if (game.level == 5){
        //     mvprintw(1, 1, "%d %d %d %d", x1, x2, y1, y2);
        //     refresh();
        //     sleep(2);
        // }
        if (l.treasure_room[i]){
            attron(A_BOLD);
            attron(COLOR_PAIR(2));
        }
        else{
            attron(A_BOLD);
            attron(COLOR_PAIR(4));
        }
        for (int x = x1; x <= x2; x++){
            mvprintw(y1, x, "=");
            mvprintw(y2, x, "=");
        }
        for (int y = y1; y <= y2; y++){
            mvprintw(y, x1, "|");
            mvprintw(y, x2, "|");
        }
        if (l.treasure_room[i]){
            attroff(COLOR_PAIR(2));
        }
        else{
            attroff(COLOR_PAIR(4));
        }
        for (int j = 0; j < l.rooms[i].number_of_doors; j++){
            if (mark[l.rooms[i].other_room[j]] && l.rooms[i].password_door[j]){
                if (l.rooms[i].open_password_door[j]){
                    attron(COLOR_PAIR(8));
                    mvprintw(l.rooms[i].doors_y[j], l.rooms[i].doors_x[j], "@");
                    attroff(COLOR_PAIR(8));
                }
                else{
                    attron(COLOR_PAIR(13));
                    mvprintw(l.rooms[i].doors_y[j], l.rooms[i].doors_x[j], "@");
                    attroff(COLOR_PAIR(13));
                }
                continue;
            } 
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
                if (game.level < 4 && l.rooms[i].in[x - x1][y - y1] == 1){
                    attron(COLOR_PAIR(9));
                    mvprintw(y, x, "S");
                    attroff(COLOR_PAIR(9));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 2){
                    attron(COLOR_PAIR(14));
                    mvprintw(y, x, "&");
                    attroff(COLOR_PAIR(14));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 4){
                    attron(COLOR_PAIR(15));
                    mvprintw(y, x, "*");
                    attroff(COLOR_PAIR(15));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 5){
                    attron(COLOR_PAIR(18));
                    mvprintw(y, x, "◍");
                    attroff(COLOR_PAIR(18));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 6){
                    attron(COLOR_PAIR(2));
                    mvprintw(y, x, "$");
                    attroff(COLOR_PAIR(2));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 7){
                    attron(COLOR_PAIR(22));
                    mvprintw(y, x, "$");
                    attroff(COLOR_PAIR(22));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 8){
                    attron(COLOR_PAIR(22));
                    mvprintw(y, x, "🛠️");
                    attroff(COLOR_PAIR(22));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 9){
                    attron(COLOR_PAIR(22));
                    mvprintw(y, x, "🗡️");
                    attroff(COLOR_PAIR(22));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 10){
                    attron(COLOR_PAIR(22));
                    mvprintw(y, x, "M");
                    attroff(COLOR_PAIR(22));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 11){
                    attron(COLOR_PAIR(22));
                    mvprintw(y, x, "↳");
                    attroff(COLOR_PAIR(22));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 12){
                    attron(COLOR_PAIR(22));
                    mvprintw(y, x, "☭");
                    attroff(COLOR_PAIR(22));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 13){
                    attron(COLOR_PAIR(24));
                    mvprintw(y, x, "✬");
                    attroff(COLOR_PAIR(24));
                }else if (l.rooms[i].in[x - x1][y - y1] == 14){
                    attron(COLOR_PAIR(25));
                    mvprintw(y, x, "✬");
                    attroff(COLOR_PAIR(25));
                }
                else if (l.rooms[i].in[x - x1][y - y1] == 15){
                    attron(COLOR_PAIR(26));
                    mvprintw(y, x, "✬");
                    attroff(COLOR_PAIR(26));
                }
                else{
                    if (l.treasure_room[i]){
                        attron(COLOR_PAIR(2));
                    }
                    else{
                        attron(COLOR_PAIR(8));
                    }
                    mvprintw(y, x, ".");
                    if (l.treasure_room[i]){
                        attroff(COLOR_PAIR(2));
                    }
                    else{
                        attroff(COLOR_PAIR(8));
                    }
                }
            }
        }
    }
    refresh();
}
void print_player(){
    attron(COLOR_PAIR(game.player_color));
    mvprintw(game.player.y, game.player.x, "P");
    attroff(COLOR_PAIR(game.player_color));
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
                    else if(game.m){
                        mvprintw(y, x, "#");
                    }
                }
            }
        }
    }
}
void print_monsters(){
    int cnt = game.level;
    level_struct l = game.levels[cnt];
    cnt = l.number_of_rooms;
    for (int i = 0; i < cnt; i++){
        if (!l.rooms[i].vis && !game.m) continue;
        if (l.rooms[i].monster.alive){
            attron(COLOR_PAIR(23));
            mvprintw(l.rooms[i].monster.y, l.rooms[i].monster.x, "%c", l.rooms[i].monster.name);
            attroff(COLOR_PAIR(23));
        }
    }
    if (game.level == 5){
        level_struct l = game.levels[5];
        cnt = l.number_of_rooms;
        for (int i = 0; i < cnt; i++){
            if (!l.rooms[i].vis && !game.m) continue;
            for (int k = 0; k < 300; k++){
                if (l.rooms[i].last_level_monsters[k].alive){
                    attron(COLOR_PAIR(23));
                    mvprintw(l.rooms[i].last_level_monsters[k].y, l.rooms[i].last_level_monsters[k].x, "%c", l.rooms[i].last_level_monsters[k].name);
                    attroff(COLOR_PAIR(23));
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
    print_monsters();
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
    for (int i = game.energy; i < 30; i++){
        attron(COLOR_PAIR(11) | A_DIM);
        mvprintw(45, x_enery++, "˩");
        attroff(COLOR_PAIR(11) | A_DIM);
    }
    attron(COLOR_PAIR(2));
    attron(A_BOLD);
    if (game.cur_weapon) mvprintw(45, 95, "current weapon: %s", weapon_name[game.cur_weapon]);
    else mvprintw(45, 95, "current weapon: NO WEAPON");
    attroff(COLOR_PAIR(2));
    attroff(A_BOLD);
}
int in_corridor(int xp, int yp){
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
                    if (game.vis_corridors[game.level][x][y] || (!inroom(game.player.x, game.player.y) && dist(xp, yp, x, y) <= 0)){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
int valid(int x, int y){
    level_struct l = game.levels[game.level];
    //monster
    // for (int i = 0; i < l.number_of_rooms; i++){
    //     if (dist(l.rooms[i].monster.x, l.rooms[i].monster.y, x, y) == 0) return 0;
    // }
    ///
    int inroom = 0;
    for (int i = 0; i < l.number_of_rooms; i++){
        bool check_door = 0;
        for (int j = 0; j < l.rooms[i].number_of_doors; j++){
            if (x == l.rooms[i].doors_x[j] && y == l.rooms[i].doors_y[j] && (!l.rooms[i].password_door[j] || l.rooms[i].password)) check_door = 1;
            else if (x == l.rooms[i].doors_x[j] && y == l.rooms[i].doors_y[j] && l.rooms[i].password_door[j] && !l.rooms[i].password){
                mvprintw(1, 1, "hi");
                if (in_corridor(game.player.x, game.player.y)) return 1;
                attron(COLOR_PAIR(12));
                attron(A_BOLD);
                mvprintw(2, 27, "THIS DOOR REQUIRES A PASSWORD. FIND IT!");
                attroff(COLOR_PAIR(12));
                attroff(A_BOLD);
                refresh();
                sleep(2);
                return 0;
            }
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
    //monster
    for (int i = 0; i < l.number_of_rooms; i++){
        if (l.rooms[i].monster.alive && dist(l.rooms[i].monster.x, l.rooms[i].monster.y, x, y) == 0) return 0;
    }
    ///
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
        move_monster(-1, -1);
        if (game.time_since_last_speed_potion <= 10 && valid(game.player.x - 1, game.player.y - 1)){
            game.player.x--, game.player.y--;
        }
    }
    else if (ch == 'u' && valid(game.player.x + 1, game.player.y - 1)){
        game.player.x++, game.player.y--;
        move_monster(1, -1);
        if (game.time_since_last_speed_potion <= 10 && valid(game.player.x + 1, game.player.y - 1)){
            game.player.x++, game.player.y--;
        }
    }
    else if (ch == 'b' && valid(game.player.x - 1, game.player.y + 1)){
        game.player.x--, game.player.y++;
        move_monster(-1, 1);
        if (game.time_since_last_speed_potion <= 10 && valid(game.player.x - 1, game.player.y + 1)){
            game.player.x--, game.player.y++;
        }
    }
    else if (ch == 'n' && valid(game.player.x + 1, game.player.y + 1)){
        game.player.x++, game.player.y++;
        move_monster(1, 1);
        if (game.time_since_last_speed_potion <= 10 && valid(game.player.x + 1, game.player.y + 1)){
            game.player.x++, game.player.y++;
        }
    }
    else if (ch == 'h' && valid(game.player.x - 1, game.player.y)){
        game.player.x--;
        move_monster(-1, 0);
        if (game.time_since_last_speed_potion <= 10 && valid(game.player.x - 1, game.player.y)){
            game.player.x--;
        }
    }
    else if (ch == 'j' && valid(game.player.x, game.player.y - 1)){
        game.player.y--;
        move_monster(0, -1);
        if (game.time_since_last_speed_potion <= 10 && valid(game.player.x, game.player.y - 1)){
            game.player.y--;
        }
    }
    else if (ch == 'k' && valid(game.player.x, game.player.y + 1)){
        game.player.y++;
        move_monster(0, 1);
        if (game.time_since_last_speed_potion <= 10 && valid(game.player.x, game.player.y + 1)){
            game.player.y++;
        }
    }
    else if (ch == 'l' && valid(game.player.x + 1, game.player.y)){
        game.player.x++;
        move_monster(1, 0);
        if (game.time_since_last_speed_potion <= 10 && valid(game.player.x + 1, game.player.y)){
            game.player.x++;
        }
    }
    else if (ch == 'f'){
        move_f();
    }
    else if (ch == 's'){
        if (!user.guest) save_game();
        endwin();
        exit(0);
    }
    else if (ch == 'M'){
        game.m = !game.m;
    }
    else if (ch == 'E'){
        inventory_menu();
    }
    else if (ch == 'i'){
        weapon_menu();
    }
    else if (ch == 'Y'){
        potion_menu();
    }
    else if (ch == 'W'){
        game.inventory.weapon_cnt[game.cur_weapon]++;
        game.cur_weapon = 0;
    }
    else if (ch == ' '){
        fight();
        move_monster(0, 0);
    }
    else if (ch == 'a'){
        fight_a();
    }
    else if (ch == 'g'){
        move_g = 1;
    }
    else if (ch == 'c'){
        game.player.x = game.levels[game.level].stairs_x;
        game.player.y = game.levels[game.level].stairs_y;
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
        update_player_state();
        return;
    }
    // else if (ch == '<' && game.player.x == game.levels[game.level].stairs_x && game.player.y == game.levels[game.level].stairs_y && game.level > 0){
    //     game.level--;
    //     return;
    // }
    else{
        move_monster(0, 0);
        move_player();
    }
}
void update_player_state(){
    int x = game.player.x, y = game.player.y;
    game.moves++;
    game.time_since_last_damage_potion++, game.time_since_last_health_potion++, game.time_since_last_speed_potion++;
    if (game.difficulty <= 1){
        if ((game.moves) % 20 == 0 && game.energy) game.energy--;
        if ((game.moves) % 10 == 15 && game.energy < 7) game.health--;
    }
    else{
        if ((game.moves) % 15 == 0 && game.energy) game.energy--;
        if ((game.moves) % 5 == 15 && game.energy < 7) game.health--;
    }
    game.time_since_last_attack++;
    if (game.energy == 30 && game.time_since_last_attack >= 30){
        game.health++;
        if (game.time_since_last_health_potion <= 10) game.health++;
        if (game.health > 100) game.health = 100;
    }
    game.vis_corridors[game.level][x][y] = 1;
    //new room
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            if (l.rooms[i].y1 <= y && y <= l.rooms[i].y2 && l.rooms[i].x1 <= x && x <= l.rooms[i].x2){
                if (!game.levels[game.level].rooms[i].vis){
                    attron(COLOR_PAIR(12));
                    attron(A_BOLD);
                    mvprintw(2, 27, "YOU HAVE ENTERED A NEW ROOM");
                    refresh();
                    sleep(1);
                    attroff(COLOR_PAIR(12));
                    attroff(A_BOLD);
                }
                game.levels[game.level].rooms[i].vis = 1;
            }
        }
    }
    // password_door
    {
        level_struct l = game.levels[game.level];
        int mark[9];
        for (int i = 0; i < 9; i++) mark[i] = 0;
        for (int i = 0; i < l.number_of_rooms; i++){
            mark[l.room_numbers[i]] = 1;
        }
        echo();
        for (int i = 0; i < l.number_of_rooms; i++){
            for (int j = 0; j < l.rooms[i].number_of_doors; j++){
                if (mark[l.rooms[i].other_room[j]] && game.player.x == l.rooms[i].doors_x[j] && game.player.y == l.rooms[i].doors_y[j]){
                    if (l.rooms[i].password_door[j] && !l.rooms[i].open_password_door[j] && l.rooms[i].password){
                        attron(COLOR_PAIR(3));
                        attron(A_BOLD);
                        mvprintw(2, 27, "ENTER THE 4-digit PASSWORD THEN PRESS ENTER: ");
                        refresh();
                        attroff(COLOR_PAIR(3));
                        attroff(A_BOLD);
                        int p;
                        scanw("%d", &p);
                        if (p != l.rooms[i].password){
                            attron(COLOR_PAIR(15));
                            attron(A_BOLD);
                            mvprintw(2, 27, "WARNING: WRONG PASSWORD. 2 ATTEMPTS REMAINING");
                            refresh();
                            sleep(1);
                            attroff(COLOR_PAIR(15));
                            attroff(A_BOLD);
                            attron(COLOR_PAIR(3));
                            attron(A_BOLD);
                            mvprintw(2, 27, "ENTER THE 4-digit PASSWORD THEN PRESS ENTER: ");
                            refresh();
                            attroff(COLOR_PAIR(3));
                            attroff(A_BOLD);
                            int p;
                            scanw("%d", &p);
                            if (p != l.rooms[i].password){
                                attron(COLOR_PAIR(16));
                                attron(A_BOLD);
                                mvprintw(2, 27, "WARNING: WRONG PASSWORD. 1 ATTEMPT REMAINING");
                                refresh();
                                sleep(1);
                                attroff(COLOR_PAIR(16));
                                attroff(A_BOLD);
                                attron(COLOR_PAIR(3));
                                attron(A_BOLD);
                                mvprintw(2, 27, "ENTER THE 4-digit PASSWORD THEN PRESS ENTER: ");
                                refresh();
                                attroff(COLOR_PAIR(3));
                                attroff(A_BOLD);
                                int p;
                                scanw("%d", &p);
                                if (p != l.rooms[i].password){
                                    //security_room();
                                }
                                else{
                                    game.levels[game.level].rooms[i].open_password_door[j] = 1;
                                }
                            }
                            else{
                                game.levels[game.level].rooms[i].open_password_door[j] = 1;
                            }
                        }
                        else{
                            game.levels[game.level].rooms[i].open_password_door[j] = 1;
                        }
                    }
                }
            }
        }
        noecho();
    }
    //password_generator
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            room_struct r = l.rooms[i];
            for (int x = r.x1 + 1; x < r.x2; x++){
                for (int y = r.y1 + 1; y < r.y2; y++){
                    if (r.in[x - r.x1][y - r.y1] == 2){
                        if (game.player.x == x && game.player.y == y){
                            int p = rand() % 8999;
                            p += 1000;
                            game.levels[game.level].rooms[i].password = p;
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            mvprintw(2, 27, "THE PASSWORD IS: %d", p);
                            refresh();
                            sleep(7);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                        }
                    }
                }
            }
        }
    }
    //trap
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            for (int xx = l.rooms[i].x1 + 1; xx < l.rooms[i].x2; xx++){
                for (int yy = l.rooms[i].y1 + 1; yy < l.rooms[i].y2; yy++){
                    if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == 3){
                        if (x == xx && y == yy){
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            mvprintw(2, 27, "OOPS! YOU FELL INTO A HIDDEN TRAP!");
                            refresh();
                            sleep(2);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                            game.health --;;
                            if (game.health < 0) game.health = 0;
                            game.levels[game.level].rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] = 0;
                            // fight_room();
                        }
                    }
                }
            }
        }
    }
    //treasure
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            for (int xx = l.rooms[i].x1 + 1; xx < l.rooms[i].x2; xx++){
                for (int yy = l.rooms[i].y1 + 1; yy < l.rooms[i].y2; yy++){
                    if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == 4){
                        if (x == xx && y == yy){
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            mvprintw(2, 27, "YOU ENTERED THE LAST LEVEL");
                            refresh();
                            sleep(2);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                            game.level++;
                            game.player.x = 90, game.player.y = 22;
                            update_player_state();
                        }
                    }
                }
            }
        }
    }
    //normal food
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            if (move_g) break;
            for (int xx = l.rooms[i].x1 + 1; xx < l.rooms[i].x2; xx++){
                for (int yy = l.rooms[i].y1 + 1; yy < l.rooms[i].y2; yy++){
                    if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == 5){
                        if (x == xx && y == yy && game.inventory.number_of_foods < 5){
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            mvprintw(2, 27, "YOU PICKED UP A FOOD");
                            refresh();
                            sleep(1);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                            game.levels[game.level].rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] = 0;
                            game.inventory.foods[game.inventory.number_of_foods] = 5;
                            game.inventory.number_of_foods++;
                        }
                    }
                }
            }
        }
    }
    //gold
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            if (move_g) break;
            for (int xx = l.rooms[i].x1 + 1; xx < l.rooms[i].x2; xx++){
                for (int yy = l.rooms[i].y1 + 1; yy < l.rooms[i].y2; yy++){
                    if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == 6){
                        if (x == xx && y == yy){
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            int x = rand() % 4 + 1;
                            mvprintw(2, 27, "YOU PICKED %d GOLDS", x);
                            game.golds += x;
                            refresh();
                            sleep(1);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                            game.levels[game.level].rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] = 0;
                        }
                    }
                }
            }
        }
    }
    //black gold
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            if (move_g) break;
            for (int xx = l.rooms[i].x1 + 1; xx < l.rooms[i].x2; xx++){
                for (int yy = l.rooms[i].y1 + 1; yy < l.rooms[i].y2; yy++){
                    if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == 7){
                        if (x == xx && y == yy){
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            int x = rand() % 4 + 10;
                            mvprintw(2, 27, "YOU PICKED %d GOLDS!", x);
                            game.golds += x;
                            refresh();
                            sleep(1);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                            game.levels[game.level].rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] = 0;
                        }
                    }
                }
            }
        }
    }
    //weapon
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            if (move_g) break;
            for (int xx = l.rooms[i].x1 + 1; xx < l.rooms[i].x2; xx++){
                for (int yy = l.rooms[i].y1 + 1; yy < l.rooms[i].y2; yy++){
                    for (int j = 8; j <= 12; j++){
                        if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == j){
                            if (x == xx && y == yy){
                                attron(COLOR_PAIR(12));
                                attron(A_BOLD);
                                int x = rand() % 4 + 10;
                                mvprintw(2, 27, "YOU PICKED A WEAPON!");
                                if (l.rooms[i].used_weapon[xx - l.rooms[i].x1][yy - l.rooms[i].y1]) game.inventory.weapon_cnt[j]++;
                                else game.inventory.weapon_cnt[j] += weapon_num[j];
                                refresh();
                                sleep(1);
                                attroff(COLOR_PAIR(12));
                                attroff(A_BOLD);
                                game.levels[game.level].rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    //next to monster
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            if (l.rooms[i].monster.alive == 0) continue;
            if (dist(l.rooms[i].monster.x, l.rooms[i].monster.y, x, y) <= 1 && !l.rooms[i].monster.touch){
                game.levels[game.level].rooms[i].monster.touch = 1;
                game.health -= l.rooms[i].monster.damage;
                if (game.health < 0) game.health = 0;
                game.time_since_last_attack = 0;
                attron(COLOR_PAIR(12));
                attron(A_BOLD);
                mvprintw(2, 27, "OOPS, YOU WAKED UP THE MONSTER. RUN!");
                refresh();
                sleep(1);
                attroff(COLOR_PAIR(12));
                attroff(A_BOLD);
            }
            else if (dist(l.rooms[i].monster.x, l.rooms[i].monster.y, x, y) <= 1){
                attron(COLOR_PAIR(12));
                attron(A_BOLD);
                mvprintw(2, 27, "THE MONSTER ATTACKED YOU. RUN!");
                refresh();
                sleep(1);
                attroff(COLOR_PAIR(12));
                attroff(A_BOLD);
                game.health -= l.rooms[i].monster.damage;
                if (game.health < 0) game.health = 0;
                game.time_since_last_attack = 0;
            }
        }
    }
    //potions
    {
        level_struct l = game.levels[game.level];
        for (int i = 0; i < l.number_of_rooms; i++){
            if (move_g) break;
            for (int xx = l.rooms[i].x1 + 1; xx < l.rooms[i].x2; xx++){
                for (int yy = l.rooms[i].y1 + 1; yy < l.rooms[i].y2; yy++){
                    if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == 13){
                        if (x == xx && y == yy){
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            mvprintw(2, 27, "YOU PICKED UP A HEALTH POTION");
                            refresh();
                            sleep(2);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                            game.levels[game.level].rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] = 0;
                            game.inventory.potion_cnt[13]++;
                        }
                    }
                    if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == 14){
                        if (x == xx && y == yy){
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            mvprintw(2, 27, "YOU PICKED UP A SPEED POTION");
                            refresh();
                            sleep(2);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                            game.levels[game.level].rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] = 0;
                            game.inventory.potion_cnt[14]++;
                        }
                    }
                    if (l.rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] == 15){
                        if (x == xx && y == yy){
                            attron(COLOR_PAIR(12));
                            attron(A_BOLD);
                            mvprintw(2, 27, "YOU PICKED UP A DAMAGE POTION");
                            refresh();
                            sleep(2);
                            attroff(COLOR_PAIR(12));
                            attroff(A_BOLD);
                            game.levels[game.level].rooms[i].in[xx - l.rooms[i].x1][yy - l.rooms[i].y1] = 0;
                            game.inventory.potion_cnt[15]++;
                        }
                    }
                }
            }
        }
    }
    //last_level
    if (game.level == 5)
    {
        level_struct l = game.levels[game.level];
        int check = 0;
        for (int i = 0; i < l.number_of_rooms; i++){
            for (int k = 0; k < 300; k++){
                if (l.rooms[i].last_level_monsters[k].alive == 0) continue;
                check = 1;
                if (dist(l.rooms[i].last_level_monsters[k].x, l.rooms[i].last_level_monsters[k].y, x, y) <= 1 && !l.rooms[i].last_level_monsters[k].touch){
                    game.levels[game.level].rooms[i].last_level_monsters[k].touch = 1;
                    game.health -= l.rooms[i].last_level_monsters[k].damage;
                    if (game.health < 0) game.health = 0;
                    game.time_since_last_attack = 0;
                    attron(COLOR_PAIR(12));
                    attron(A_BOLD);
                    mvprintw(2, 27, "OOPS, YOU WAKED UP THE MONSTER. RUN!");
                    refresh();
                    sleep(1);
                    attroff(COLOR_PAIR(12));
                    attroff(A_BOLD);
                }
                else if (dist(l.rooms[i].last_level_monsters[k].x, l.rooms[i].last_level_monsters[k].y, x, y) <= 1){
                    attron(COLOR_PAIR(12));
                    attron(A_BOLD);
                    mvprintw(2, 27, "THE MONSTER ATTACKED YOU. RUN!");
                    refresh();
                    sleep(1);
                    attroff(COLOR_PAIR(12));
                    attroff(A_BOLD);
                    game.health -= l.rooms[i].last_level_monsters[k].damage;
                    if (game.health < 0) game.health = 0;
                    game.time_since_last_attack = 0;
                }
            }
        }
        if (!check){
            attron(COLOR_PAIR(12));
            attron(A_BOLD);
            mvprintw(2, 27, "Winner, winner, chicken dinner!");
            refresh();
            sleep(5);
            attroff(COLOR_PAIR(12));
            attroff(A_BOLD);
            win();
        }
    }
    if (move_g == 2) move_g = 0;
    if (move_g == 1) move_g = 2;
}
void win(){
    user.gold += game.golds;
    user.games++;
    user.score += game.score;
    save_user();
    endwin();
    exit(0);
}
void lose(){
    mvprintw(2, 27, "The journey ends here... For now.");
    refresh();
    sleep(4);
    user.gold += game.golds;
    user.games++;
    save_user();
    endwin();
    exit(0);
}
void save_user(){
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.save", user.username);
    FILE *file = fopen(filename, "wb");
    size_t result = fwrite(&user, sizeof(user_struct), 1, file);
    fclose(file);
}
void draw_inventory_border(){
    attron(COLOR_PAIR(17));
    for (int x = 60; x <= 130; x++){
        for (int y = 10; y <= 40; y++){
            mvprintw(y, x, " ");
        }
    }
    attroff(COLOR_PAIR(17));
    attron(COLOR_PAIR(19));
    for (int y = 10; y <= 40; y++){
        mvprintw(y, 60, "*");
        mvprintw(y, 130, "*");
    }
    for (int x = 60; x <= 130; x++){
        mvprintw(10, x, "*");
        mvprintw(40, x, "*");
    }
    attroff(COLOR_PAIR(19));
}
void inventory_menu(){
    int curmenu = 0;
    int total_menus;
    while(1){
        total_menus = game.inventory.number_of_foods + 1;
        draw_inventory_border();
        attron(COLOR_PAIR(5));
        attron(A_BOLD);
        mvprintw(11, 90, "INVENTORY");
        attroff(COLOR_PAIR(5));
        attroff(A_BOLD);
        for (int i = 0; i < total_menus - 1; i++){
            if (curmenu == i){
                if (game.inventory.foods[i] == 5){
                    attron(COLOR_PAIR(21));
                    mvprintw(13 + i, 92, "NORMAL FOOD (ADDS 1 TO ENERGY)");
                    attroff(COLOR_PAIR(21));
                }
            }
            else{
                if (game.inventory.foods[i] == 5){
                    attron(COLOR_PAIR(5));
                    mvprintw(13 + i, 90, "NORMAL FOOD");
                    attroff(COLOR_PAIR(5));
                }
            }
        }
        if (curmenu == total_menus - 1){
            attron(COLOR_PAIR(21));
            attron(A_BLINK);
            mvprintw(12 + total_menus, 93, "Exit");
            attroff(COLOR_PAIR(21));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(5));
            attron(A_BLINK);
            mvprintw(12 + total_menus, 93, "Exit");
            attroff(COLOR_PAIR(5));
            attroff(A_BLINK);
        }
        refresh();
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
            for (int i = 0; i < total_menus - 1; i++){
                if (curmenu == i){
                    for (int j = i; j < game.inventory.number_of_foods; j++){
                        game.inventory.foods[j] = game.inventory.foods[j + 1];
                    }
                    game.energy++;
                    if (game.energy > 30) game.energy = 30;
                    game.inventory.number_of_foods--;
                    print_game();
                }
            }
            if (curmenu == total_menus - 1){
                return;
            }
        }
    }
}
void weapon_menu(){
    int curmenu = 0;
    int total_menus;
    while(1){
        clear();
        print_game();
        total_menus = 1;
        for (int i = 0; i < 20; i++){
            if (game.inventory.weapon_cnt[i]) total_menus++;
        }
        draw_inventory_border();
        attron(COLOR_PAIR(5));
        attron(A_BOLD);
        mvprintw(11, 90, "INVENTORY");
        attroff(COLOR_PAIR(5));
        attroff(A_BOLD);
        for (int i = 0; i < total_menus - 1; i++){
            if (curmenu == i){
                attron(COLOR_PAIR(21));
                int cnt = 0;
                for (int j = 0; j < 20; j++){
                    if (game.inventory.weapon_cnt[j]){
                        if (i != cnt){
                            cnt++;
                            continue;
                        }
                        if (j == 8) mvprintw(13 + i, 89, "MACE 🛠️ (range 1 damage 5) : %d", game.inventory.weapon_cnt[j]);
                        else if (j == 9) mvprintw(13 + i, 89, "DAGGER 🗡️ (range 5 damage 12) : %d", game.inventory.weapon_cnt[j]);
                        else if (j == 10) mvprintw(13 + i, 89, "MAGIC WAND M (range 10 damage 15) : %d", game.inventory.weapon_cnt[j]);
                        else if (j == 11) mvprintw(13 + i, 89, "NORMAL ARROW ↳ (range 5 damage 5) : %d", game.inventory.weapon_cnt[j]);
                        else if (j == 12) mvprintw(13 + i, 89, "SWORD ☭ (range 1 damage 10): %d", game.inventory.weapon_cnt[j]);
                        cnt++;
                    }
                }
                attroff(COLOR_PAIR(21));
            }
            else{
                attron(COLOR_PAIR(5));
                int cnt = 0;
                for (int j = 0; j < 20; j++){
                    if (game.inventory.weapon_cnt[j]){
                        if (i != cnt){
                            cnt++;
                            continue;
                        }
                        if (j == 8) mvprintw(13 + i, 88, "MACE 🛠️ (range 1 damage 5) : %d", game.inventory.weapon_cnt[j]);
                        else if (j == 9) mvprintw(13 + i, 88, "DAGGER 🗡️ (range 5 damage 12) : %d", game.inventory.weapon_cnt[j]);
                        else if (j == 10) mvprintw(13 + i, 88, "MAGIC WAND M (range 10 damage 15) : %d", game.inventory.weapon_cnt[j]);
                        else if (j == 11) mvprintw(13 + i, 88, "NORMAL ARROW ↳ (range 5 damage 5) : %d", game.inventory.weapon_cnt[j]);
                        else if (j == 12) mvprintw(13 + i, 88, "SWORD ☭ (range 1 damage 10): %d", game.inventory.weapon_cnt[j]);
                        cnt++;
                    }
                }
                attroff(COLOR_PAIR(5));
            }
        }
        if (curmenu == total_menus - 1){
            attron(COLOR_PAIR(21));
            attron(A_BLINK);
            mvprintw(12 + total_menus, 94, "Exit");
            attroff(COLOR_PAIR(21));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(5));
            attron(A_BLINK);
            mvprintw(12 + total_menus, 93, "Exit");
            attroff(COLOR_PAIR(5));
            attroff(A_BLINK);
        }
        refresh();
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
            for (int i = 0; i < total_menus - 1; i++){
                if (curmenu == i){
                    int cnt = 0;
                    for (int j = 0; j < 20; j++){
                        if (game.inventory.weapon_cnt[j]){
                            if (i != cnt){
                                cnt++;
                                continue;
                            }
                            pick_weapon(j);
                            return;
                        }
                    }
                }
            }
            if (curmenu == total_menus - 1){
                return;
            }
        }
    }
}
void move_monster(int mx, int my){
    level_struct l = game.levels[game.level];
    for (int i = 0; i < l.number_of_rooms; i++){
        int x1 = l.rooms[i].x1, x2 = l.rooms[i].x2, y1 = l.rooms[i].y1, y2 = l.rooms[i].y2;
        monster_struct m = l.rooms[i].monster;
        if (!m.alive) continue;
        if (m.touch && m.len == 0){
            game.levels[game.level].rooms[i].monster.len = m.lensave;
            game.levels[game.level].rooms[i].monster.touch = 0;
            continue;;
        }
        if (x1 <= game.player.x && game.player.x <= x2 && y1 <= game.player.y && game.player.y <= y2){
            if (!m.touch){
                int check = 0;
                while(!check){
                    int arr[6] = {-1, 1, 0, 0, 0, 0};
                    int val = rand() % 6;
                    int dir = rand() % 2;
                    int xx, yy;
                    if (dir == 0) xx = 0, yy = arr[val];
                    else yy = 0, xx = arr[val];
                    if (x1 < m.x + xx && m.x + xx < x2 && y1 < m.y + yy && m.y + yy < y2){
                        game.levels[game.level].rooms[i].monster.x += xx;
                        game.levels[game.level].rooms[i].monster.y += yy;
                        check = 1;
                    }
                }
            }
            if (!m.alive || !m.len || !m.touch) continue;
            if (x1 < m.x + mx && m.x + mx < x2 && y1 < m.y + my && m.y + my < y2 && !(m.x + mx == game.player.x && m.y + my == game.player.y)){
                game.levels[game.level].rooms[i].monster.x += mx;
                game.levels[game.level].rooms[i].monster.y += my;
                game.levels[game.level].rooms[i].monster.len--;
            }
        }
    }
    if (game.level == 5){
        for (int i = 0; i < l.number_of_rooms; i++){
            int x1 = l.rooms[i].x1, x2 = l.rooms[i].x2, y1 = l.rooms[i].y1, y2 = l.rooms[i].y2;
            for (int k = 0; k < 300; k++){
                monster_struct m = l.rooms[i].last_level_monsters[k];
                if (!m.alive) continue;
                if (m.touch && m.len == 0){
                    game.levels[game.level].rooms[i].last_level_monsters[k].len = m.lensave;
                    game.levels[game.level].rooms[i].last_level_monsters[k].touch = 0;
                    continue;;
                }
                if (x1 <= game.player.x && game.player.x <= x2 && y1 <= game.player.y && game.player.y <= y2){
                    if (!m.touch){
                        int check = 0;
                        while(!check){
                            int arr[6] = {-1, 1, 0, 0, 0, 0};
                            int val = rand() % 6;
                            int dir = rand() % 2;
                            int xx, yy;
                            if (dir == 0) xx = 0, yy = arr[val];
                            else yy = 0, xx = arr[val];
                            if (x1 < m.x + xx && m.x + xx < x2 && y1 < m.y + yy && m.y + yy < y2 && !(m.x + xx == 49 && m.y + yy == 34)){
                                game.levels[game.level].rooms[i].last_level_monsters[k].x += xx;
                                game.levels[game.level].rooms[i].last_level_monsters[k].y += yy;
                                check = 1;
                            }
                        }
                    }
                    if (!m.alive || !m.len || !m.touch) continue;
                    if (x1 < m.x + mx && m.x + mx < x2 && y1 < m.y + my && m.y + my < y2 && !(m.x + mx == game.player.x && m.y + my == game.player.y) && !(m.x + mx == 49 && m.y + my == 34)){
                        game.levels[game.level].rooms[i].last_level_monsters[k].x += mx;
                        game.levels[game.level].rooms[i].last_level_monsters[k].y += my;
                        game.levels[game.level].rooms[i].last_level_monsters[k].len--;
                    }
                }
            }
        }
    }
}
void pick_weapon(int ind){
    clear();
    print_game();
    if (game.cur_weapon){
        attron(COLOR_PAIR(12));
        attron(A_BOLD);
        mvprintw(2, 27, "YOU ALREADY HAVE A WEAPON. PUT IT IN YOUR BACKPACK BY PRESSING W");
        refresh();
        sleep(2);
        attroff(COLOR_PAIR(12));
        attroff(A_BOLD);
        return;
    }
    attron(COLOR_PAIR(12));
    attron(A_BOLD);
    mvprintw(2, 27, "YOU CHANGED YOUR WEAPON TO %s", weapon_name[ind]);
    refresh();
    sleep(2);
    attroff(COLOR_PAIR(12));
    attroff(A_BOLD);
    game.inventory.weapon_cnt[ind]--;
    game.cur_weapon = ind;
    
}
void fight(){
    int w = game.cur_weapon;
    if (w == 0){
        attron(COLOR_PAIR(12));
        attron(A_BOLD);
        mvprintw(2, 27, "PICK A WEAPON FIRST");
        refresh();
        sleep(1);
        attroff(COLOR_PAIR(12));
        attroff(A_BOLD);
        return;
    }
    int x = game.player.x;
    int y = game.player.y;
    if (weapon_dist[w] == 1){
        monster_damage(x, y - 1); monster_damage(x - 1, y); monster_damage(x, y + 1); monster_damage(x + 1, y);
        monster_damage(x - 1, y - 1); monster_damage(x + 1, y - 1); monster_damage(x - 1, y + 1); monster_damage(x + 1, y + 1);
        if (game.cur_weapon != 8 && game.cur_weapon != 12) game.inventory.weapon_cnt[game.cur_weapon] --;
        if (game.cur_weapon != 8 && game.cur_weapon != 12 && !game.inventory.weapon_cnt[game.cur_weapon]){
            game.cur_weapon = 0;
            attron(COLOR_PAIR(12));
            attron(A_BOLD);
            mvprintw(2, 27, "OUT OF WEAPON");
            refresh();
            sleep(1);
            attroff(COLOR_PAIR(12));
            attroff(A_BOLD);
        }
        last_fight = 'a';
    }
    else{
        int c = getch();
        last_fight = c;
        if (c == KEY_UP){
            shoot_line(0, -1, weapon_dist[game.cur_weapon]);
        }
        else if (c == KEY_DOWN){
            shoot_line(0, 1, weapon_dist[game.cur_weapon]);
        }
        else if (c == KEY_RIGHT){
            shoot_line(1, 0, weapon_dist[game.cur_weapon]);
        }
        else if (c == KEY_LEFT){
            shoot_line(-1, 0, weapon_dist[game.cur_weapon]);
        }
        if (game.cur_weapon != 8 && game.cur_weapon != 12) game.inventory.weapon_cnt[game.cur_weapon] --;
        if (game.cur_weapon != 8 && game.cur_weapon != 12 && !game.inventory.weapon_cnt[game.cur_weapon]){
            game.cur_weapon = 0;
            attron(COLOR_PAIR(12));
            attron(A_BOLD);
            mvprintw(2, 27, "OUT OF WEAPON");
            refresh();
            sleep(1);
            attroff(COLOR_PAIR(12));
            attroff(A_BOLD);
        }
    }
}
int monster_damage(int x, int y){
    level_struct l = game.levels[game.level];
    for (int i = 0; i < l.number_of_rooms; i++){
        if (!l.rooms[i].monster.alive) continue;
        if (x == l.rooms[i].monster.x && y == l.rooms[i].monster.y){
            game.levels[game.level].rooms[i].monster.health -= weapon_damage[game.cur_weapon];
            if (game.time_since_last_damage_potion <= 10) game.levels[game.level].rooms[i].monster.health -= weapon_damage[game.cur_weapon];
            if (game.cur_weapon == 10){
                game.levels[game.level].rooms[i].monster.len = 0;
            }
            if (l.rooms[i].monster.health <= 0){
                l.rooms[i].monster.health = 0;
                game.levels[game.level].rooms[i].monster.alive = 0;
            }
            attron(COLOR_PAIR(12));
            attron(A_BOLD);
            mvprintw(2, 27, "YOU HIT %c . Current health : %d", l.rooms[i].monster.name, l.rooms[i].monster.health);
            refresh();
            sleep(1);
            attroff(COLOR_PAIR(12));
            attroff(A_BOLD);
            return 1;
        }
    }
    if (game.level == 5){
        for (int i = 0; i < l.number_of_rooms; i++){
            for (int k = 0; k < 300; k++){
                if (!l.rooms[i].last_level_monsters[k].alive) continue;
                if (x == l.rooms[i].last_level_monsters[k].x && y == l.rooms[i].last_level_monsters[k].y){
                    game.levels[game.level].rooms[i].last_level_monsters[k].health -= weapon_damage[game.cur_weapon];
                    if (game.time_since_last_damage_potion <= 10) game.levels[game.level].rooms[i].last_level_monsters[k].health -= weapon_damage[game.cur_weapon];
                    if (game.cur_weapon == 10){
                        game.levels[game.level].rooms[i].last_level_monsters[k].len = 0;
                    }
                    if (l.rooms[i].last_level_monsters[k].health <= 0){
                        l.rooms[i].last_level_monsters[k].health = 0;
                        game.levels[game.level].rooms[i].last_level_monsters[k].alive = 0;
                    }
                    attron(COLOR_PAIR(12));
                    attron(A_BOLD);
                    mvprintw(2, 27, "YOU HIT %c . Current health : %d", l.rooms[i].last_level_monsters[k].name, l.rooms[i].last_level_monsters[k].health);
                    refresh();
                    sleep(1);
                    attroff(COLOR_PAIR(12));
                    attroff(A_BOLD);
                    return 1;
                }
            }
        }
    }
    return 0;
}
void shoot_line(int xx, int yy, int d){
    int x = game.player.x, y = game.player.y;
    while(d && !is_wall(x + xx, y + yy)){
        x += xx;
        y += yy;
        if (game.cur_weapon == 8) mvprintw(y, x, "🛠️");
        else if (game.cur_weapon == 9) mvprintw(y, x, "🗡️");
        else if (game.cur_weapon == 10) mvprintw(y, x, "M");
        else if (game.cur_weapon == 11) mvprintw(y, x, "↳");
        else if (game.cur_weapon == 12) mvprintw(y, x, "☭"); 
        refresh();
        sleep(1);
        if (monster_damage(x, y)) return;
        d--;
    }
    put_weapon(x, y);
}
int is_wall(int x, int y){
    level_struct l = game.levels[game.level];
    for (int i = 0; i < l.number_of_rooms; i++){
        room_struct r = l.rooms[i];
        if ((x == r.x1 || x == r.x2) && r.y1 <= y && y <= r.y2) return 1;
        if ((y == r.y1 || y == r.y2) && r.x1 <= x && x <= r.x2) return 1;
    }
    return 0;
}
void put_weapon(int x, int y){
    level_struct l = game.levels[game.level];
    for (int i = 0; i < l.number_of_rooms; i++){
        room_struct r = l.rooms[i];
        if (r.y1 <= y && y <= r.y2 && r.x1 <= x && x <= r.x2){
            game.levels[game.level].rooms[i].in[x - r.x1][y - r.y1] = game.cur_weapon;
            game.levels[game.level].rooms[i].used_weapon[x - r.x1][y - r.y1] = 1;
        }
    }
}
void fight_a(){
    int w = game.cur_weapon;
    if (w == 0){
        attron(COLOR_PAIR(12));
        attron(A_BOLD);
        mvprintw(2, 27, "PICK A WEAPON FIRST");
        refresh();
        sleep(1);
        attroff(COLOR_PAIR(12));
        attroff(A_BOLD);
        return;
    }
    int x = game.player.x;
    int y = game.player.y;
    if (weapon_dist[w] == 1){
        monster_damage(x, y - 1); monster_damage(x - 1, y); monster_damage(x, y + 1); monster_damage(x + 1, y);
        monster_damage(x - 1, y - 1); monster_damage(x + 1, y - 1); monster_damage(x - 1, y + 1); monster_damage(x + 1, y + 1);
        if (game.cur_weapon != 8 && game.cur_weapon != 12) game.inventory.weapon_cnt[game.cur_weapon] --;
        if (game.cur_weapon != 8 && game.cur_weapon != 12 && !game.inventory.weapon_cnt[game.cur_weapon]){
            game.cur_weapon = 0;
            attron(COLOR_PAIR(12));
            attron(A_BOLD);
            mvprintw(2, 27, "OUT OF WEAPON");
            refresh();
            sleep(1);
            attroff(COLOR_PAIR(12));
            attroff(A_BOLD);
        }
    }
    else if (last_fight != 'a'){
        int c = last_fight;
        if (c == KEY_UP){
            shoot_line(0, -1, weapon_dist[game.cur_weapon]);
        }
        else if (c == KEY_DOWN){
            shoot_line(0, 1, weapon_dist[game.cur_weapon]);
        }
        else if (c == KEY_RIGHT){
            shoot_line(1, 0, weapon_dist[game.cur_weapon]);
        }
        else if (c == KEY_LEFT){
            shoot_line(-1, 0, weapon_dist[game.cur_weapon]);
        }
        if (game.cur_weapon != 8 && game.cur_weapon != 12) game.inventory.weapon_cnt[game.cur_weapon] --;
        if (game.cur_weapon != 8 && game.cur_weapon != 12 && !game.inventory.weapon_cnt[game.cur_weapon]){
            game.cur_weapon = 0;
            attron(COLOR_PAIR(12));
            attron(A_BOLD);
            mvprintw(2, 27, "OUT OF WEAPON");
            refresh();
            sleep(1);
            attroff(COLOR_PAIR(12));
            attroff(A_BOLD);
        }
    }
}
void potion_menu(){
    int curmenu = 0;
    int total_menus;
    while(1){
        clear();
        print_game();
        total_menus = 1;
        for (int i = 0; i < 20; i++){
            if (game.inventory.potion_cnt[i]) total_menus++;
        }
        draw_inventory_border();
        attron(COLOR_PAIR(5));
        attron(A_BOLD);
        mvprintw(11, 90, "INVENTORY");
        attroff(COLOR_PAIR(5));
        attroff(A_BOLD);
        for (int i = 0; i < total_menus - 1; i++){
            if (curmenu == i){
                attron(COLOR_PAIR(21));
                int cnt = 0;
                for (int j = 0; j < 20; j++){
                    if (game.inventory.potion_cnt[j]){
                        if (i != cnt){
                            cnt++;
                            continue;
                        }
                        if (j == 13) mvprintw(13 + i, 89, "Health Potion : %d", game.inventory.potion_cnt[j]);
                        else if (j == 14) mvprintw(13 + i, 89, "Speed Potion : %d", game.inventory.potion_cnt[j]);
                        else if (j == 15) mvprintw(13 + i, 89, "Damage Potion : %d", game.inventory.potion_cnt[j]);
                        cnt++;
                    }
                }
                attroff(COLOR_PAIR(21));
            }
            else{
                attron(COLOR_PAIR(5));
                int cnt = 0;
                for (int j = 0; j < 20; j++){
                    if (game.inventory.potion_cnt[j]){
                        if (i != cnt){
                            cnt++;
                            continue;
                        }
                        if (j == 13) mvprintw(13 + i, 89, "Health Potion : %d", game.inventory.potion_cnt[j]);
                        else if (j == 14) mvprintw(13 + i, 89, "Speed Potion : %d", game.inventory.potion_cnt[j]);
                        else if (j == 15) mvprintw(13 + i, 89, "Damage Potion : %d", game.inventory.potion_cnt[j]);
                        cnt++;
                    }
                }
                attroff(COLOR_PAIR(5));
            }
        }
        if (curmenu == total_menus - 1){
            attron(COLOR_PAIR(21));
            attron(A_BLINK);
            mvprintw(12 + total_menus, 94, "Exit");
            attroff(COLOR_PAIR(21));
            attroff(A_BLINK);
        }
        else{
            attron(COLOR_PAIR(5));
            attron(A_BLINK);
            mvprintw(12 + total_menus, 93, "Exit");
            attroff(COLOR_PAIR(5));
            attroff(A_BLINK);
        }
        refresh();
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
            for (int i = 0; i < total_menus - 1; i++){
                if (curmenu == i){
                    int cnt = 0;
                    for (int j = 0; j < 20; j++){
                        if (game.inventory.potion_cnt[j]){
                            if (i != cnt){
                                cnt++;
                                continue;
                            }
                            pick_potion(j);
                            return;
                        }
                    }
                }
            }
            if (curmenu == total_menus - 1){
                return;
            }
        }
    }
}
void pick_potion(int ind){
    if (ind == 13){
        game.time_since_last_health_potion = 0;
    }
    else if (ind == 14){
        game.time_since_last_speed_potion = 0;
    }
    else if (ind == 15){
        game.time_since_last_damage_potion = 0;
    }
}