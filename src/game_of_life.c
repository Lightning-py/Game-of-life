#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FIELD_HEIGHT 25
#define FIELD_WIDTH 80

int** allocate(void);
int** create_matrix(void);
int get_alive_neighbours(int** field, int x, int y);

void update_field(int** field);
void display(int** field);

int get_right_index_x(int x);
int get_right_index_y(int y);

void free_field(int** array);
void display_hello(void);

int main(void) {
    int** matr = create_matrix();
    int speed = 100000;

    setlocale(LC_ALL, "");

    if (freopen("/dev/tty", "r", stdin)) initscr();

    noecho();

    nodelay(stdscr, TRUE);

    cbreak();

    curs_set(0);

    char ch = ' ';
    do {
        ch = getch();
        update_field(matr);
        if (speed < 200000 && (ch == 'm' || ch == 'M')) speed += 10000;
        if (speed > 10000 && (ch == 'k' || ch == 'K')) speed -= 10000;

        clear();
        usleep(speed);
        display_hello();
        display(matr);
        printw("\nspeed == %d, k and m for change speed", 21 - (speed / 10000));

        refresh();

    } while (ch != 'q' && ch != 'Q');
    free_field(matr);
    endwin();
    return 0;
}

void update_field(int** field) {
    int** new_array = allocate();

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            int nb = get_alive_neighbours(field, j, i);

            if ((field[i][j]) == 0) {
                if (nb == 3) new_array[i][j] = 1;
            } else {
                if (nb == 2 || nb == 3) {
                    new_array[i][j] = 1;
                } else
                    new_array[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            (field)[i][j] = new_array[i][j];
        }
    }

    free_field(new_array);
}

int get_alive_neighbours(int** field, int x, int y) {
    int count = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int x_ind = get_right_index_x(x + dx);
            int y_ind = get_right_index_y(y + dy);

            if (field[y_ind][x_ind] == 1) count++;
        }
    }

    return count;
}

int get_right_index_x(int x) {
    if (x == -1) return FIELD_WIDTH - 1;
    if (x == FIELD_WIDTH) return 0;
    return x;
}

int get_right_index_y(int y) {
    if (y == -1) return FIELD_HEIGHT - 1;
    if (y == FIELD_HEIGHT) return 0;
    return y;
}

void display(int** field) {
    for (int i = 0; i < FIELD_WIDTH + 2; ++i) printw("-");
    printw("\n");

    for (int i = 0; i < FIELD_HEIGHT / 2; ++i) {
        printw("|");
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            const char* map[4] = {" ", "▄", "▀", "█"};  // block characters
            int index = ((field[i * 2][j] & 1) << 1)    // top pixel active
                        | (field[i * 2 + 1][j] & 1);    // bottom pixel active
            printw(map[index]);
        }
        printw("|");
        printw("\n");
    }

    for (int i = 0; i < FIELD_WIDTH + 2; ++i) printw("-");
}

void display_hello() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    printw("                                             ___    _ _ ___     \n");
    printw("                    ___ ___ _____ ___    ___|  _|  | |_|  _|___ \n");

    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));

    printw("                   | . | .'|     | -_|  | . |  _|  | | |  _| -_|\n");

    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));

    printw("                   |_  |__,|_|_|_|___|  |___|_|    |_|_|_| |___|\n");
    printw("                   |___|                                        \n");

    attroff(COLOR_PAIR(3));
}

int** allocate() {
    int** field = calloc(FIELD_HEIGHT, sizeof(int*));
    if (field == NULL) {
        printw("memory error");
        free(field);
        exit(0);
    }

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        field[i] = calloc(FIELD_WIDTH, sizeof(int));

        if (field[i] == NULL) {
            for (int j = 0; j < FIELD_HEIGHT; ++j) free(field[j]);

            free(field);

            printw("memory error");
            exit(0);
        }
    }

    return field;
}

int** create_matrix() {
    int** field = allocate();

    char c;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            c = getchar();

            if (c == '0')
                field[i][j] = 0;
            else if (c == '1')
                field[i][j] = 1;
            else {
                free_field(field);

                printw("input error");
                exit(0);
            }
        }
        getchar();
    }

    return field;
}

void free_field(int** array) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        free(array[i]);
    }
    free(array);
}
