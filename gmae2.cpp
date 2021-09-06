#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<windows.h>
#include<conio.h>
void setcolor(int fg, int bg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void draw_ship(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setcolor(5, 2);
    printf(" <-0-> ");

}
void erase_ship(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setcolor(0, 0);
    printf("       ");
}
void setcursor(bool visible)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 20;
    SetConsoleCursorInfo(console, &lpCursor);
}
void draw_bullet(int x, int y)
{
    COORD c = { x+1, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setcolor(4, 0);
    printf("  ^  ");
}
void erase_bullet(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setcolor(0, 0);
    printf("     ");
}
struct Bullet {
    int active = false;
    int x = 0, y = 0;
};


int main()
{
    char ch = ' ';
    int x = 38, y = 27;
    int bullet = 0;
    setcursor(0);
    int left = -1;
    int right = 1;
    int stop = 0;
    int direction = 0;
    Bullet bullets[5];
    draw_ship(x, y);
    do {
        if (_kbhit())
        {
            ch = _getch();
            if (ch == 'a')
            {
                direction = left;
            }
            if (ch == 'd')
            {
                direction = right;
            }
            if (ch == 's')
            {
                direction = stop;
            }
            if (ch == ' ')
            {
                for (int i = 0; i < 5; i++)
                {
                    if (bullets[i].active == false)
                    {
                        bullets[i].active = true;
                        bullets[i].x = x;
                        bullets[i].y = y;
                        break;
                    }
                }
            }
            
      
            fflush(stdin);
        }
        for (int i = 0; i < 5; i++)
        {
            if (bullets[i].active)
            {
                erase_bullet(bullets[i].x, bullets[i].y);
                if (bullets[i].y > 0)
                {
                    draw_bullet(bullets[i].x, --bullets[i].y);
                }
                else
                {
                    bullets[i].active = false;
                }
            }
        }
        if(direction == left && x > 0)
        {
            erase_ship(x, y); draw_ship(--x, y);
        }
        else if (direction == right && x < 74)
        {
            erase_ship(x, y); draw_ship(++x, y);
        }
        else
        {
            draw_ship(x, y);
        }
        Sleep(50);
    } while (ch != 'x');
    return 0;
}