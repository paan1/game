#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
int score = 0;
void setcolor(int fg, int bg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void gotoxy(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
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
char cursor(int x, int y)
{
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[2]; COORD c = { x,y }; DWORD num_read;
    if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
        return '\0';
    else
        return buf[0];
}
void draw_bullet(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setcolor(4, 4);
    printf("^");
}
void erase_bullet(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setcolor(0, 0);
    printf(" ");
}
struct Bullet {
    int active = false;
    int x = 0, y = 0;
}bullets[5];
struct Star
{
    int active = false;
    int x = 0, y = 0;

}star[20];
void draw_Star(int x, int y)
{
    gotoxy(x, y);
    setcolor(3, 4);
    printf("*");
}
void erase_Star(int x, int y)
{
    gotoxy(x, y);
    setcolor(0, 0);
    printf(" ");
}
void chek_star(int x, int y,int i)
{
    
    char chek = cursor(x, y-1);
    if (chek == '*')
    {
        bullets[i].active = false;
        erase_Star(x, y-1);
        draw_Star(rand() % 70 + 10, rand() % 6 + 2);
        erase_bullet(x, y);
        Beep(700, 100);
        score++;
    }
}
void draw_score(int x , int y,int score)
{
    gotoxy(x, y);
    setcolor(3, 0);
    printf("Score : %d", score);
}



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
    draw_ship(x, y);
    for (int i = 0; i < 20; i++)
    {
        star[i].x = rand()%70+10;
        star[i].y = rand()%6+2;
        draw_Star(star[i].x,star[i].y);
       
    }
    do {
        draw_score(85, 2, score);
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
                        bullets[i].x = x+3;
                        bullets[i].y = y;
                     
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < 5; i++)
        {
            if (bullets[i].active)
            {
                erase_bullet(bullets[i].x, bullets[i].y);
                if (bullets[i].y > 0)
                {
                    draw_bullet(bullets[i].x, --bullets[i].y);
                    chek_star(bullets[i].x, bullets[i].y,i);
                }
                else
                {
                    bullets[i].active = false;

                }
            }
        }
        if (direction == left && x > 0)
        {
            erase_ship(x, y); draw_ship(--x, y);
        }
        else if (direction == right && x < 75)
        {
            erase_ship(x, y); draw_ship(++x, y);
        }
        else
        {
            draw_ship(x, y);
        }
        Sleep(40);
    } while (ch != 'x');
    return 0;
}