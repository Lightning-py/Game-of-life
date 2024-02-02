#include <stdio.h>
#include <stdlib.h>

/*
 * компилировать код только с флагами -Wall -Werror -Wextra
 *
 */

int** create_matrix()  // создает массив игрового поля 80x25 клеток и читает поле со стандартного ввода
int get_alive_neighbours(int x,
                             int y)  // возвращает количество живых клеток вокруг данной, принимает координаты
                                     // клетки (необходимо учитывать то, что края поля соединены)
void update_field(int*** field)  // расчитывает следующий шаг игры и записывает в int*** field
void display(int** field)  // отображает текущий шаг на экране

int main() {
    return 0;
}
