#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_HEIGHT 25
#define FIELD_WIDTH 80

/*
 * компилировать код только с флагами -Wall -Werror -Wextra
 *
 */

int** allocate();       // выделяет память для массива
int** create_matrix();  // создает массив игрового поля 80x25 клеток и читает поле со стандартного ввода
int get_alive_neighbours(int** field, int x,
                         int y);  // возвращает количество живых клеток вокруг данной, принимает
                                  // координаты клетки (необходимо учитывать то, что края поля соединены)
void update_field(int** field);  // расчитывает следующий шаг игры и записывает в int*** field
void display(int** field);  // отображает текущий шаг на экране

int get_right_index_x(int x);
int get_right_index_y(int y);

void free_field(int** array);  // освобождает память

int main() {
    int** matr = create_matrix();

    int steps = 0;

    while (steps < 2) {
        update_field(matr);

        display(matr);

        delay(10);
        steps++;
    }

    return 0;
}

void update_field(int** field) {
    int** new_array = allocate();

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            int nb = get_alive_neighbours(field, j, i);

            if ((field)[i][j] == 0)  // dead
            {
                if (nb == 3) new_array[i][j] = 1;
            } else  // alive
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
            (field)[i][j] = new_array[i][j];
        }
    }

    free_field(new_array);
}

int get_alive_neighbours(int** field, int x, int y) {
    int count = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
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
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            if (field[i][j] == 1)
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
}

int** allocate() {
    int** field = calloc(FIELD_HEIGHT, sizeof(int*));

    if (field == NULL) {
        printf("memoy error");
        exit(0);
    }

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        field[i] = calloc(FIELD_WIDTH, sizeof(int));

        if (field[i] == NULL) {
            printf("memoy error");
            exit(0);
        }
    }

    return field;
}

int** create_matrix() {
    // -- create

    int** field = allocate();

    // -- input

    char c = ' ';
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            c = getchar();

            if (c == '0' || c == ' ') {
                field[i][j] = 0;
            } else if (c == '1' || c == '*') {
                field[i][j] = 1;
            } else {
                free_field(field);

                printf("input error");
                exit(0);
            }
        }
        c = getchar();
    }

    return field;
}

void free_field(int** array) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        free(array[i]);
    }

    free(array);
}