#include <iostream>
#include <Windows.h>::Sleep

#include "player.h"
#include "point2D.h"
#include "Map.h"
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

//int playerX = 0;
//int player.position.y = 0;

char playerChar = '@';
char ground = '.';

bool isRunning = true;

void generateBlankMap(Map *map);
void playerMove();
void displayMap();

#include <windows.h>


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
	//Main game loop here, loop until the player asks to quit
	//currentMap = generateBlankMap(currentMap);
	currentMap.pointer = &currentMap;

	generateBlankMap(currentMap.pointer);

	while (isRunning) {
		displayMap();
		printf("Current Pos: %d, %d \n", player.position.x, player.position.y);
		playerMove();
		ClearScreen();
	}

	system("PAUSE");
	return 0;
}

void generateBlankMap(Map *map)
{
	//Add ground to map

	for (int y = 0; y < (*map).height; y++)
	{
		for (int x = 0; x < (*map).width; x++)
		{
			(*map).map[x][y] = ground;
		}
	}

	//Add player to pos
	(*map).map[player.position.x][player.position.y] = playerChar;
}

void displayMap()
{
	//Print out map
	for (int y = currentMap.height-1; y >= 0; y--)
	{
		for (int x = 0; x < currentMap.width; x++)
		{
			std::cout << currentMap.map[x][y];
		}
		std::cout << std::endl;
	}

}

char input = ' ';
//char inputArray[2];

char lastInput = ' ';

void inputSwitch();

void playerMove()
{
	currentMap.map[player.position.x][player.position.y] = ground;
	printf("Move player: \n");

	std::cin >> input;
	//std::cin.get(inputArray, 2);
	//input = inputArray[0];

	inputSwitch();

	currentMap.map[player.position.x][player.position.y] = playerChar;
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