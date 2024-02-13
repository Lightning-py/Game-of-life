#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FIELD_HEIGHT 25
#define FIELD_WIDTH 80

int** allocate(int HEIGHT, int WIDTH);
int** create_matrix(int HEIGHT, int WIDTH);
int get_alive_neighbours(int** field, int x, int y, int HEIGHT, int WIDTH);

void update_field(int** field, int HEIGHT, int WIDTH);
void display(int** field, int HEIGHT, int WIDTH);

int get_right_index_x(int x, int WIDTH);
int get_right_index_y(int y, int HEIGHT);

void free_field(int** array, int HEIGHT);
void display_hello(void);

int main(int argc, char* argv[]) {
    int WIDTH = FIELD_WIDTH, HEIGHT = FIELD_HEIGHT;

    if (argc == 3) {
        if (!(sscanf(argv[1], "%d", &HEIGHT) == 1 && sscanf(argv[2], "%d", &WIDTH) == 1)) {
            printf("invalid command string arguments\n");
            WIDTH = FIELD_WIDTH;
            HEIGHT = FIELD_HEIGHT;
        }
    }

    int** matr = create_matrix(HEIGHT, WIDTH);
    printf("lol");
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
        update_field(matr, HEIGHT, WIDTH);
        if (speed < 200000 && (ch == 'm' || ch == 'M')) speed += 10000;
        if (speed > 10000 && (ch == 'k' || ch == 'K')) speed -= 10000;

        clear();
        usleep(speed);
        display_hello();
        display(matr, HEIGHT, WIDTH);
        printw("\nspeed == %d, k and m for change speed", 21 - (speed / 10000));

        refresh();

    } while (ch != 'q' && ch != 'Q');
    free_field(matr, HEIGHT);
    endwin();
    return 0;
}

void update_field(int** field, int HEIGHT, int WIDTH) {
    int** new_array = allocate(HEIGHT, WIDTH);

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            int nb = get_alive_neighbours(field, j, i, HEIGHT, WIDTH);

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

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            (field)[i][j] = new_array[i][j];
        }
    }

    free_field(new_array, HEIGHT);
}

int get_alive_neighbours(int** field, int x, int y, int HEIGHT, int WIDTH) {
    int count = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int x_ind = get_right_index_x(x + dx, WIDTH);
            int y_ind = get_right_index_y(y + dy, HEIGHT);

            if (field[y_ind][x_ind] == 1) count++;
        }
    }

    return count;
}

int get_right_index_x(int x, int WIDTH) {
    if (x == -1) return WIDTH - 1;
    if (x == WIDTH) return 0;
    return x;
}

int get_right_index_y(int y, int HEIGHT) {
    if (y == -1) return HEIGHT - 1;
    if (y == HEIGHT) return 0;
    return y;
}


void display(int** field, int HEIGHT, int WIDTH) {
    for (int i = 0; i < (WIDTH / 2) + 2; ++i) printw("-");
    printw("\n");

    for (int i = 0; i < HEIGHT / 4; ++i) {
        printw("|");
        for (int j = 0; j < WIDTH / 2; ++j) {
            int ib = i * 4;  // i base
            int jb = j * 2;  // j base
            wchar_t braille = 0x2800;
            if (field[ib + 0][jb + 0]) braille += 0x01;
            if (field[ib + 1][jb + 0]) braille += 0x02;
            if (field[ib + 2][jb + 0]) braille += 0x04;
            if (field[ib + 3][jb + 0]) braille += 0x40;
            if (field[ib + 0][jb + 1]) braille += 0x08;
            if (field[ib + 1][jb + 1]) braille += 0x10;
            if (field[ib + 2][jb + 1]) braille += 0x20;
            if (field[ib + 3][jb + 1]) braille += 0x80;
            printw("%lc", braille);

        }
        printw("|");
        printw("\n");
    }



    for (int i = 0; i < WIDTH + 2; ++i) printw("-");

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

int** allocate(int HEIGHT, int WIDTH) {
    int** field = calloc(HEIGHT, sizeof(int*));
    if (field == NULL) {
        printw("memory error");
        free(field);
        exit(0);
    }

    for (int i = 0; i < HEIGHT; ++i) {
        field[i] = calloc(WIDTH, sizeof(int));

        if (field[i] == NULL) {
            for (int j = 0; j < HEIGHT; ++j) free(field[j]);

            free(field);

            printw("memory error");
            exit(0);
        }
    }

    return field;
}

int** create_matrix(int HEIGHT, int WIDTH) {
    int** field = allocate(HEIGHT, WIDTH);

    char c;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            c = getchar();

            if (c == '0')
                field[i][j] = 0;
            else if (c == '1')
                field[i][j] = 1;
            else {
                free_field(field, HEIGHT);

                printw("input error");
                exit(0);
            }
        }
        getchar();
    }

    return field;
}

void free_field(int** array, int HEIGHT) {
    for (int i = 0; i < HEIGHT; ++i) {
        free(array[i]);
    }
    free(array);
}
