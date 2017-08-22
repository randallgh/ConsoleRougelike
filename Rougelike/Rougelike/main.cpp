#include <iostream>
#include <Windows.h>::Sleep
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

int playerX = 0;
int playerY = 0;

char playerChar = '@';

int mapWidth = 10;
int mapHeight = 10;

char ground = '.';
char map[10][10] = {};

bool isRunning = true;

void generateMap();
void playerMove();
void displayMap();

int main()
{
	//Main game loop here, loop until the player asks to quit



	generateMap();

	while (isRunning) {
		displayMap();
		playerMove();
	}

	system("PAUSE");
	return 0;
}

void generateMap()
{
	//Add ground to map

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			map[x][y] = '.';
		}
	}

	//Add player to pos
	map[playerX][playerY] = playerChar;
}

void displayMap()
{
	//Print out map
	for (int y = mapHeight-1; y >= 0; y--)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			std::cout << map[x][y];
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
	map[playerX][playerY] = ground;
	printf("Move player: \n");

	std::cin >> input;
	//std::cin.get(inputArray, 2);
	//input = inputArray[0];

	printf("Player Cords: %d, %d \n", playerX, playerY);

	inputSwitch();

	printf("Player Cords: %d, %d \n", playerX, playerY);

	map[playerX][playerY] = playerChar;
}

void inputSwitch()
{
	switch (input)
	{
	case 'w':
	case 'W':
		//Move north
		playerY++;
		if (playerY > mapHeight - 1) {
			playerY--;
		}
		lastInput = 'w';
		break;
	case 'a':
	case 'A':
		//Move west
		playerX--;
		if (playerX < 0) {
			playerX++;
		}
		lastInput = 'a';
		break;
	case 's':
	case 'S':
		//Move south
		playerY--;
		if (playerY < 0) {
			playerY++;
		}
		lastInput = 's';
		break;
	case 'd':
	case 'D':
		//Move east
		playerX++;
		if (playerX > mapWidth - 1) {
			playerX--;
		}
		lastInput = 'd';
		break;
	case 'e':
	case 'E':
		isRunning = false;
		break;
	case ' ':
		inputSwitch();
		break;
	default:
		break;
	}
}