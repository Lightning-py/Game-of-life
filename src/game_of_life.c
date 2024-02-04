#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FIELD_HEIGHT 25
#define FIELD_WIDTH 80

#define DEAD_CELL " "
#define LIVE_CELL "*"

#define speed_1 100000
#define speed_2 50000
#define speed_3 25000
#define speed_4 15000
#define speed_5 7500
#define speed_6 3500

/*
 * компилировать код только с флагами -Wall -Werror -Wextra -lncurses
 */

int** allocate(void);       // выделяет память для массива
int** create_matrix(void);  // создает массив игрового поля 80x25 клеток и читает поле со стандартного ввода
int get_alive_neighbours(int** field, int x,
                         int y);  // возвращает количество живых клеток вокруг данной, принимает
                                  // координаты клетки (необходимо учитывать то, что края поля соединены)
void update_field(int** field);  // расчитывает следующий шаг игры и записывает в int*** field
void display(int** field);  // отображает текущий шаг на экране

int get_right_index_x(int x);
int get_right_index_y(int y);

void free_field(int** array);  // освобождает память
int main(void) {
    int** matr = create_matrix();
    // инициализация библиотеку curses
    initscr();
    // при вводе, вводимые символы не будут видны
    noecho();
    // что оно делает вообще
    nodelay(stdscr, FALSE);
    // cbreak отменяет действие raw (полный контроль клавиатуры)
    cbreak();
    // убирает курсор
    curs_set(0);

    while (1) {
        update_field(matr);

        clear();
        printw("Game of life\n");
        display(matr);
        printw("\nhello world");
        refresh();

        usleep(speed_3);
    }

    endwin();
    return 0;
}

void update_field(int** field) {
    // создаем новую матрицу
    int** new_array = allocate();

    // идем по матрице
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            // смотрим количество соседей
            int nb = get_alive_neighbours(field, j, i);

            if ((field[i][j]) == 0)  // dead inside
            {
                if (nb == 3) new_array[i][j] = 1;
            } else  // alive outside
            {
                if (nb == 2 || nb == 3) {
                    new_array[i][j] = 1;
                } else
                    new_array[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            // почему в скобочках переменная, второй раз такое вижу
            (field)[i][j] = new_array[i][j];
        }
    }

    free_field(new_array);
}
// передаем матрицу и координаты
int get_alive_neighbours(int** field, int x, int y) {
    int count = 0;

    // итерируемся по квадратику 3 на 3
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            // если мы в серединке, то пропускаем
            if (dx == 0 && dy == 0) continue;
            int x_ind = get_right_index_x(x + dx);
            int y_ind = get_right_index_y(y + dy);
            // если в этой точке есть клетка, то обновляем счетчик
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

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        printw("|");
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field[i][j] == 1)
                printw(LIVE_CELL);
            else
                printw(DEAD_CELL);
        }
        printw("|");
        printw("\n");
    }

    for (int i = 0; i < FIELD_WIDTH + 2; ++i) printw("-");
}

int** allocate() {
    // создание указателя на указатель на память в размере длины матрицы?
    int** field = calloc(FIELD_HEIGHT, sizeof(int*));
    // если не получилось выделить паммять, выводим что все пропало и выходим из проги?
    if (field == NULL) {
        printw("memory error");
        // это из какой библиотеки?
        exit(0);
    }
    // идем от нуля до количества строк массива
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        // выделяем память для каждого массива длинной в количество колонок
        field[i] = calloc(FIELD_WIDTH, sizeof(int));
        // чистим память хули
        if (field[i] == NULL) {
            for (int j = 0; j < FIELD_HEIGHT; ++j) free(field[j]);

            free(field);

            printw("memory error");
            exit(0);
        }
    }
    // возвращаем матрицу
    return field;
}

int** create_matrix() {
    // -- create

    int** field = allocate();

    // -- input

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
