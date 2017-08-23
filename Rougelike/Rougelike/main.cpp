#include <iostream>
#include <Windows.h>

#include "Player.h"
#include "Point2D.h"
#include "Map.h"

#define WIDTH 70
#define HEIGHT 35
/*
Map data/class?
File IO
Player data
	Player pos
	Player inventory
	Player stats
Console display class

LATER(Extra Features)
Camera class			
Item data/class
*/

Player player;
Map currentMap;

bool isRunning = true;

void playerMove();
void displayMap();

HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

/* A CHAR_INFO structure containing data about a single character */
CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

/* Window size coordinates, be sure to start index at zero! */
SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT - 1 };

/* A COORD struct for specificying the console's screen buffer dimensions */
COORD bufferSize = { WIDTH, HEIGHT };

/* Setting up different variables for passing to WriteConsoleOutput */
COORD characterBufferSize = { WIDTH, HEIGHT };
COORD characterPosition = { 0, 0 };
SMALL_RECT consoleWriteArea = { 0, 0, WIDTH - 1, HEIGHT - 1 };

/* http://www.cplusplus.com/forum/articles/10515/ */
void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,

		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

int main()
{
	/* initialize handles */
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	/* Set the console's title */
	SetConsoleTitle("Rougelike");

	/* Set the window size */
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	/* Set the screen's buffer size */
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	//Main game loop here, loop until the player asks to quit

	//Make a pointer for the map
	currentMap.pointer = &currentMap;
	//generateBlankMap(currentMap.pointer, player);
	generateBlankFlatMap(currentMap.pointer, player);

	while (isRunning) {
		displayMap();
		//printf("Current Pos: %d, %d \n", player.position.x, player.position.y);
		playerMove();
		ClearScreen();
	}

	system("PAUSE");
	return 0;
}

void displayMap()
{
	//Print out map

	//for (int y = currentMap.height-1; y >= 0; y--)
	//{
	//	for (int x = 0; x < currentMap.width; x++)
	//	{
	//		std::cout << currentMap.map[x][y];
	//	}
	//	std::cout << std::endl;
	//}

	int y, x;

	char tempChar = '.';
	WORD charInfo = 14;

	for (y = 0; y < HEIGHT; ++y)
	{
		for (x = 0; x < WIDTH; ++x)
		{
			//Get the char from the map
			//tempChar = currentMap.map[x][(HEIGHT - 1) - y];
			tempChar = currentMap.data[x + WIDTH * ((HEIGHT - 1) - y)];

			switch (tempChar)
			{
			case '.':
				charInfo = 10;
				tempChar = '.';
				break;
			case '@':
				charInfo = 15;
				tempChar = '@';
				break;
			default:
				break;
			}

			consoleBuffer[x + WIDTH * y].Char.AsciiChar = (unsigned char)tempChar;
			consoleBuffer[x + WIDTH * y].Attributes = charInfo;
		}
	}

	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}

char input = ' ';
//char inputArray[2];
char lastInput = ' ';
void inputSwitch();

void playerMove()
{
	//currentMap.map[player.position.x][player.position.y] = currentMap.ground;
	currentMap.data[flatArrayIndex(player.position, currentMap)] = currentMap.ground;

	printf("Move player: ");

	std::cin >> input;
	//std::cin.get(inputArray, 2);
	//input = inputArray[0];

	inputSwitch();

	//currentMap.map[player.position.x][player.position.y] = player.character;
	currentMap.data[flatArrayIndex(player.position, currentMap)] = player.character;
}

void inputSwitch()
{
	switch (input)
	{
	case 'w':
	case 'W':
		//Move north
		player.position.y++;
		if (player.position.y > currentMap.height - 1) {
			player.position.y--;
		}
		lastInput = 'w';
		break;
	case 'a':
	case 'A':
		//Move west
		player.position.x--;
		if (player.position.x < 0) {
			player.position.x++;
		}
		lastInput = 'a';
		break;
	case 's':
	case 'S':
		//Move south
		player.position.y--;
		if (player.position.y < 0) {
			player.position.y++;
		}
		lastInput = 's';
		break;
	case 'd':
	case 'D':
		//Move east
		player.position.x++;
		if (player.position.x > currentMap.width - 1) {
			player.position.x--;
		}
		lastInput = 'd';
		break;
	case 'e':
	case 'E':
		//Exit game
		isRunning = false;
		break;
	case ' ':
		inputSwitch();
		break;
	default:
		break;
	}
}