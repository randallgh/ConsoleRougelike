#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <chrono>

#include "Player.h"
#include "Enemy.h"

/*---------------------------------------------------------------------*/
//RENDERING
#define WIDTH 70
#define HEIGHT 35

HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

/* Window size coordinates, be sure to start index at zero! */
SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT - 1 };

/* A COORD struct for specificying the console's screen buffer dimensions */
COORD bufferSize = { WIDTH, HEIGHT };

/* Setting up different variables for passing to WriteConsoleOutput */
COORD characterBufferSize = { WIDTH, HEIGHT };
COORD characterPosition = { 0, 0 };
SMALL_RECT consoleWriteArea = { 0, 0, WIDTH - 1, HEIGHT - 1 };

/* A CHAR_INFO structure containing data about a single character */
CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

/*---------------------------------------------------------------------*/
//States

enum GameStates
{
	OVERWORLD,
	BATTLE
};

GameStates gameState = OVERWORLD;


/*---------------------------------------------------------------------*/
//DATA
bool isRunning = true;

//Map
char mapData[WIDTH * HEIGHT];

//UI
char uiData[WIDTH * HEIGHT];

//Player
Player player;
char previousChar = '.';
int previousPos = 0;

//Enemies
#define ENEMIESLENGTH 1

Enemy enemies[ENEMIESLENGTH];
Enemy slime = {20, 5, 'S'};

/*---------------------------------------------------------------------*/

void render();
void overworldInput();
void checkPlayerCollison();
void ai();


int main()
{
	srand(time(0));

	/* initialize handles */
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	/* Set the console's title */
	SetConsoleTitle("Rougelike");

	/* Set the window size */
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	/* Set the screen's buffer size */
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	//Map Generation

	for (int i = 0; i < HEIGHT * WIDTH; i++)
	{
		mapData[i] = '.';
	}

	//Add enemies to map
	enemies[0] = copyEnemy(slime);
	enemies[0].position = { 10, 10 };
	enemies[0].pointer = &enemies[0];

	//Add player to pos
	mapData[player.position.x * player.position.y] = player.character;
	player.pointer = &player;




	//Game Loop
	while (isRunning) {

		switch (gameState)
		{
		case OVERWORLD:
			//Playermovement
			overworldInput();

			//Move player
			if (player.hasMoved) {
				mapData[vectorToFlatArray(player.positionPrevious, WIDTH)] = '.';
			}

			//Move enemies
			ai();
			mapData[enemies[0].position.x + WIDTH * enemies[0].position.y] = enemies[0].character;

			//Check for collison
			//If the positons of both the enemy and player are the same COLLIDE and start battle
			checkPlayerCollison();

			mapData[player.position.x + WIDTH * player.position.y] = player.character;
			break;

		case BATTLE:


			break;
		default:
			break;
		}

		//Render
		render();

		Sleep(100);


		
	}
}

void render()
{
	char tempChar;
	int y, x;
	for (y = 0; y < HEIGHT; ++y) {
		for (x = 0; x < WIDTH; ++x) {
			consoleBuffer[x + WIDTH * y].Char.AsciiChar = mapData[x + WIDTH * ((HEIGHT - 1) - y)];
			consoleBuffer[x + WIDTH * y].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
	}

	/* Write our character buffer (a single character currently) to the console buffer */
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}

void overworldInput() {


	if (GetAsyncKeyState(VK_UP))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		setPreviousPosition(player.pointer, player.position);
		//previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.y++;

		if (player.position.y >= HEIGHT) {
			player.position.y = (HEIGHT - 1);
		}

		player.hasMoved = true;
	} else if (GetAsyncKeyState(VK_DOWN))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		setPreviousPosition(player.pointer, player.position);
		//previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.y--;

		if (player.position.y <= 0) {
			player.position.y = 0;
		}

		player.hasMoved = true;
	} else if (GetAsyncKeyState(VK_RIGHT))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		setPreviousPosition(player.pointer, player.position);
		//previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.x++;

		if (player.position.x >= WIDTH) {
			player.position.x = (WIDTH - 1);
		}

		player.hasMoved = true;
	} else if (GetAsyncKeyState(VK_LEFT))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		setPreviousPosition(player.pointer, player.position);
		//previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.x--;

		if (player.position.x < 0) {
			player.position.x = 0;
		}

		player.hasMoved = true;
	}

	if (GetAsyncKeyState(VK_ESCAPE)) {
		isRunning = false;
	}
}

void ai() 
{
	for (int i = 0; i < ENEMIESLENGTH; i++) {
		setPreviousPosition(enemies[i].pointer, enemies[i].position);
	}
}

void checkPlayerCollison() {
	//If the player and the enemy(unknown which one) have the same positon
	for (int i = 0; i < ENEMIESLENGTH; i++) {
		if ((player.position.x == enemies[i].position.x) 
			&& (player.position.y == enemies[i].position.y)) {

			player.character = 'X';
			enemies[i].character = 'O';

			setPosition(player.pointer, player.positionPrevious);
			setPosition(enemies[i].pointer, enemies[i].positionPrevious);
		}
	}
}