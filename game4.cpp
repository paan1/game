#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#define screen_x 80
#define screen_y 25
#define scount 80
HANDLE rHnd;
HANDLE wHnd;
DWORD fdwMode;
COORD star[scount];
COORD bufferSize = { screen_x,screen_y };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD characterPos = { 0,0 };
COORD ship;
DWORD color = 7;

bool play = true;
int posX = screen_x / 2;
int posY = screen_y - 1;
int check = 0;



void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
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
int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}
void draw_ship(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	char a[] = "<-0->";
	consoleBuffer[(c.X - 4) + screen_x * c.Y].Char.AsciiChar = '<';
	consoleBuffer[(c.X - 3) + screen_x * c.Y].Char.AsciiChar = '-';
	consoleBuffer[(c.X - 2) + screen_x * c.Y].Char.AsciiChar = '0';
	consoleBuffer[(c.X - 1) + screen_x * c.Y].Char.AsciiChar = '-';
	consoleBuffer[c.X + screen_x * c.Y].Char.AsciiChar = '>';
	consoleBuffer[(c.X - 4) + screen_x * c.Y].Attributes = color;
	consoleBuffer[(c.X - 3) + screen_x * c.Y].Attributes = color;
	consoleBuffer[(c.X - 2) + screen_x * c.Y].Attributes = color;
	consoleBuffer[(c.X - 1) + screen_x * c.Y].Attributes = color;
	consoleBuffer[c.X + screen_x * c.Y].Attributes = color;

}
void star_fall()
{
	int i;
	for (i = 0; i < scount; i++) 
	{
		if (star[i].Y >= screen_y - 1) 
		{
			star[i] = { (rand() % screen_x),1 };
		}
		else 
		{
			star[i] = { star[i].X,star[i].Y + 1 };
		}
	}
}
void init_star()
{
		for (int i = 0; i < scount; i++)
		{
		star[i].X = rand() % screen_x;
		star[i].Y = rand() % screen_y;		
		}
}
void clear_buffer()
{
	for (int y = 0; y < screen_y; ++y)
		{
			for (int x = 0; x < screen_x; ++x)
			{
				consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
				consoleBuffer[x + screen_x * y].Attributes = 7;
			}
		}
}
	void fill_star_to_buffer()
	{
		for (int i = 0; i < scount; i++)
		{
			consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
			consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
		}
	}
	
	void check_star()
	{
		
		for (int i = 0; i < scount; i++)
		{
			if ((star[i].X == ship.X || star[i].X == ship.X + 1 || star[i].X == ship.X + 2) && star[i].Y == ship.Y)
			{
				star[i] = { SHORT(rand() % screen_x), 1 };
				check++;
			}
			
		}
	}

	
int main()
{
	int i;
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	init_star();
	i = 0;
	bool play = true;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setConsole(screen_x, screen_y);
	setMode();
	setcursor(0);
	while (play && check <= 10)
	{
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT &&
					eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						play = false;
					}
					color = rand() % 256;
				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						FROM_LEFT_1ST_BUTTON_PRESSED) {
						color = rand() % 256;
					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
						
						posX = posx;
						posY = posy;
						
					}
				}
			}
			delete[] eventBuffer;
		}
			star_fall();
			clear_buffer();
			fill_star_to_buffer();
			draw_ship(posX, posY);
			fill_buffer_to_console();
			check_star();
			Sleep(50);
	
	}
	
	
	return 0;
}