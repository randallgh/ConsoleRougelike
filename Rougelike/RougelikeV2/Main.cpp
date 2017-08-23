#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <chrono>

#include "Player.h"

/*---------------------------------------------------------------------*/
//Rendering
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


/*---------------------------------------------------------------------*/
//Data
char mapData[WIDTH * HEIGHT];

Player player;
char previousChar = '.';
int previousPos = 0;

/*---------------------------------------------------------------------*/

void render();
void input();

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

	//Add player to pos
	mapData[0] = player.character;







	bool isRunning = true;

	//Game Loop
	while (isRunning) {

		//Playermovement

		input();

		//Move player
		if (player.hasMoved) {
			mapData[previousPos] = '.';
			mapData[player.position.x + WIDTH * player.position.y] = player.character;
		}


		render();

		Sleep(50);


		
	}
}

void render()
{
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

void input() {

	if (GetAsyncKeyState(VK_UP))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.y++;

		if (player.position.y >= HEIGHT) {
			player.position.y = (HEIGHT - 1);
		}

		player.hasMoved = true;
	} else if (GetAsyncKeyState(VK_DOWN))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.y--;

		if (player.position.y <= 0) {
			player.position.y = 0;
		}

		player.hasMoved = true;
	} else if (GetAsyncKeyState(VK_RIGHT))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.x++;

		if (player.position.x >= WIDTH) {
			player.position.x = (WIDTH - 1);
		}

		player.hasMoved = true;
	} else if (GetAsyncKeyState(VK_LEFT))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.x--;

		if (player.position.x < 0) {
			player.position.x = 0;
		}

		player.hasMoved = true;
	}
}