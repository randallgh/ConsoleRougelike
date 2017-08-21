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

void playerMove()
{
	map[playerX][playerY] = ground;
	printf("Move player: \n");
	
	char input = ' ';
	std::cin >> input;

	printf("Player Cords: %d, %d \n", playerX, playerY);
	switch (input)
	{
	case 'w':
	case 'W':
		//Move north
		playerY++;
		if (playerY > mapHeight - 1) {
			playerY--;
		}
		break;
	case 'a':
	case 'A':
		//Move west
		playerX--;
		if (playerX < 0) {
			playerX++;
		}
		break;
	case 's':
	case 'S':
		//Move south
		playerY--;
		if (playerY < 0) {
			playerY++;
		}
		break;
	case 'd':
	case 'D':
		//Move east
		playerX++;
		if (playerX > mapWidth - 1) {
			playerX--;
		}
		break;
	case 'e':
	case 'E':
		isRunning = false;
		break;
	default:
		break;
	}
	printf("Player Cords: %d, %d \n", playerX, playerY);

	map[playerX][playerY] = playerChar;
}