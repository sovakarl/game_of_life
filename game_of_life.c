#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define string 25
#define cols 80
int espace(const char *d, int x, int y);
void map(char *p);
void conf(char *data);
void game(char *data, char *data2);
int status(const char *Lapata, int x, int y);
void full(char *data, char *data2);
void rewrite(char *data, char *data2);
void change_speed(int *velocity);
int main() {
    char data[string][cols], data2[string][cols];
    char *p = (char *)data;
    char *p2 = (char *)data2;
    full(p, p2);
    conf(p);
    int velocity = 100;
    if (freopen("/dev/tty", "r", stdin)) {
        initscr();
    }
    timeout(1);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    while (1) {
        clear();
        change_speed(&velocity);
        map(p);
        game(p, p2);
        rewrite(p, p2);
        usleep(1000000 / velocity);
        refresh();
    }
    endwin();

    return 0;
}

void change_speed(int *velocity) {
    int key = getch();
    if (key == 'a') {
        *velocity = *velocity * 5;
    }
    if (key == 'z') {
        *velocity = *velocity / 5;
    }
}
void map(char *p)  // отрисовка игрового поля
{
    for (int i = 0; i < string; i++) {
        for (int j = 0; j < cols; j++) {
            printw("%c", p[(i * cols) + j]);
        }
        printw("\n");
    }
    refresh();
}
void conf(char *data)  // начальная конфигурация
{
    int Map[string][cols];
    for (int i = 0; i < string; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &Map[i][j]);
            if (Map[i][j] == 0)
                data[(i * cols) + j] = ' ';
            else if (Map[i][j] == 1)
                data[(i * cols) + j] = '*';
        }
    }
}
int espace(const char *d, int x, int y)  // поиск жизни в эпсилон окрестности
{
    int fl = 0;
    for (int i = (y - 1); i < (y + 2); i++) {
        for (int j = (x - 1); j < (x + 2); j++) {
            if (d[(((i + string) % string) * cols) + ((j + cols) % cols)] == ' ')
                continue;
            else {
                if (d[(((i + string) % string) * cols) + ((j + cols) % cols)] ==
                    '*')  // учитывает на одну точку больше ,если в координатах находится
                          // живая точка
                    fl += 1;
            }
        }
    }
    return fl;
}
void game(char *data, char *data2)  // формирование поколений
{
    for (int y = 0; y < string; y++) {
        for (int x = 0; x < cols; x++) {
            if (status(data, x, y) == 0 && espace(data, x, y) == 3)  // клетка мертвая и у нее 3 соседа
            {
                data2[((y * cols) + x)] = '*';
            } else if (status(data, x, y) == 1 &&
                       (espace(data, x, y) > 4 || espace(data, x, y) < 3))  // клетка живая и у нее больше 3
                                                                            // или меньше 2 живых соседей
            {
                data2[((y * cols) + x)] = ' ';
            } else if (status(data, x, y) == 1 &&
                       (espace(data, x, y) == 4 ||
                        espace(data, x, y) == 3))  // клетка живая и у нее 3 или 2 соседа
                data2[((y * cols) + x)] = '*';
        }
    }
}
int status(const char *Lapata, int x, int y)  // живая или мертвая клетка?
{
    int fl = 0;
    if (Lapata[(y * cols) + x] == '*') {
        fl = 1;
    }
    return fl;
}
void rewrite(char *data, char *data2) {
    for (int i = 0; i < string; i++) {
        for (int j = 0; j < cols; j++) {
            data[((i * cols) + j)] = data2[((i * cols) + j)];
            data2[((i * cols) + j)] = ' ';
        }
    }
}
void full(char *data, char *data2)  // создание пустого поля для двух масивов
{
    for (int i = 0; i < string; i++) {
        for (int j = 0; j < cols; j++) {
            data[(i * cols) + j] = ' ';
            data2[(i * cols) + j] = ' ';
        }
    }
}