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


int main(){
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
    }
    else return 0;
    menus();
    pregame();
    while(1){

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
                // new_game();
                return;
            }
            else if (curmenu == 1){
                // resume_game();
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