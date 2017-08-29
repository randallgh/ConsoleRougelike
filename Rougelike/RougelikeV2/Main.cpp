#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <chrono>

#include "Player.h"
#include "Enemy.h"

/*---------------------------------------------------------------------*/
//RENDERING
#define WIDTH 80 //70
#define HEIGHT 50 //35

#define MAPHEIGHT 40 //Uses the same width as the screen
#define UIHEIGHT 10 //Uses the same width as the screen



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


/*-------------------------------DATA----------------------------------*/
bool isRunning = true;

/*-----Map-----*/
char mapData[WIDTH * MAPHEIGHT];

char ground = 249;

/*-----UI-----*/
char uiData[WIDTH * UIHEIGHT];

Vector2D playerHealthUIStart = { 59, 8 };

/*-----Player-----*/
Player player;

/*-----Enemies-----*/
#define ENEMIESLENGTH 1

Enemy enemies[ENEMIESLENGTH];
Enemy slime = {20, 5, 'S'};

/*----Battle----*/
Enemy currentEnemy;

/*---------------------------------------------------------------------*/

void render();
bool input();
void checkPlayerCollison();
void ai();


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

	//Map Generation

	for (int i = 0; i < MAPHEIGHT * WIDTH; ++i)
	{
		mapData[i] = ground;
	}

	//UI Generation
	for (int i = 0; i < UIHEIGHT * WIDTH; ++i) 
	{
		uiData[i] = ' ';
	}

	char healthUI[] = "Player: ";
	//8 to the start
	

	//Need a line of characters at the top of the UI box
	for (int y = 0; y < UIHEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (y == 9) {
				uiData[x + WIDTH * y] = 205;
			}
			if ((x >= 0 && y >= 8) && (x <= 9 && y <= 8)) {
				uiData[x + WIDTH * y] = x + 48;
			}
			if (x == 50) {
				uiData[x + WIDTH * y] = 186;
			}
			if (x == 50 && y == 9) {
				uiData[x + WIDTH * y] = 203;
			}
			if (x == 51 && y == 8) {
				for (int i = 0; i < 8; ++i) 
				{
					uiData[x + WIDTH * y + i] = healthUI[i];
				}
			}

			
		}
	}

	//Add enemies to map
	enemies[0] = copyEnemy(slime);
	enemies[0].position = { 10, 20 };
	enemies[0].pointer = &enemies[0];

	//Add player to pos
	player.position = { 0, 0 };
	//mapData[player.position.x * player.position.y] = player.character;
	player.pointer = &player;




	/*---------------------------Game Loop--------------------------*/
	while (isRunning) {

		srand(time(0));

		switch (gameState)
		{
		case OVERWORLD:
			//Playermovement
			input();
			mapData[vectorToFlatArray(player.positionPrevious, WIDTH)] = ground;

			//Move player

			//Move enemies
			ai();

			//Check for collison
			//If the positons of both the enemy and player are the same COLLIDE and start battle
			checkPlayerCollison();

			break;

		case BATTLE:

			if (input()) {
				mapData[vectorToFlatArray(player.positionPrevious, WIDTH)] = ground;
				ai();
				checkPlayerCollison();
			}

			break;
		default:
			break;
		}

		//Render
		for (Enemy element : enemies) {
			mapData[element.position.x + WIDTH * element.position.y] = element.character;
		}
		//mapData[enemies[0].position.x + WIDTH * enemies[0].position.y] = enemies[0].character;
		mapData[player.position.x + WIDTH * player.position.y] = player.character;

		render();

		Sleep(100);
	}
}

//Takes whatever is in mapData and adds it to the console buffer
void render()
{
	char tempChar;
	int y, x;
	for (y = 0; y < HEIGHT; ++y) 
	{
		for (x = 0; x < WIDTH; ++x) 
		{
			if ((x + WIDTH * ((MAPHEIGHT - 1) - y))  >= 0) {
				consoleBuffer[x + WIDTH * y].Char.AsciiChar = mapData[x + WIDTH * ((MAPHEIGHT - 1) - y)];
				consoleBuffer[x + WIDTH * y].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			} 
			if ((x + WIDTH  * y) >= MAPHEIGHT * WIDTH ) {
				consoleBuffer[x + WIDTH * y].Char.AsciiChar = uiData[x + WIDTH * ((UIHEIGHT - 1) - (y - MAPHEIGHT))];
				consoleBuffer[x + WIDTH * y].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			}
		}
	}

	/* Write our character buffer (a single character currently) to the console buffer */
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}

bool input() {

	//North East
	if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))
	{
		setPreviousPosition(player.pointer, player.position);
		
		++player.position.y;
		++player.position.x;

		if (player.position.y >= MAPHEIGHT) {
			player.position.y = (MAPHEIGHT - 1);

		}

		if (player.position.x >= WIDTH) {
			player.position.x = (WIDTH - 1);
		}

		return true;

	}
	//South East
	else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_RIGHT))
	{
		setPreviousPosition(player.pointer, player.position);

		--player.position.y;
		++player.position.x;
		

		if (player.position.y <= 0) {
			player.position.y = 0;
		}

		if (player.position.x >= WIDTH) {
			player.position.x = (WIDTH - 1);
		}

		return true;

	}
	//South West
	else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
	{
		setPreviousPosition(player.pointer, player.position);

		--player.position.y;
		--player.position.x;

		if (player.position.y <= 0) {
			player.position.y = 0;
		}

		if (player.position.x < 0) {
			player.position.x = 0;
		}

		return true;

	}
	//North West
	else if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))
	{
		setPreviousPosition(player.pointer, player.position);

		++player.position.y;
		--player.position.x;

		if (player.position.y >= MAPHEIGHT) {
			player.position.y = (MAPHEIGHT - 1);

		}

		if (player.position.x < 0) {
			player.position.x = 0;   
		}

		return true;

	}
	//North
	else if (GetAsyncKeyState(VK_UP))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		setPreviousPosition(player.pointer, player.position);
		//previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.y++;

		if (player.position.y >= MAPHEIGHT) {
			player.position.y = (MAPHEIGHT - 1);
		}

		return true;
	}
	//South
	else if (GetAsyncKeyState(VK_DOWN))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		setPreviousPosition(player.pointer, player.position);
		//previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.y--;

		if (player.position.y <= 0) {
			player.position.y = 0;
		}

		return true;
	}
	//East
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		setPreviousPosition(player.pointer, player.position);
		//previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.x++;

		if (player.position.x >= WIDTH) {
			player.position.x = (WIDTH - 1);
		}

		return true;
	}
	//West
	else if (GetAsyncKeyState(VK_LEFT))
	{
		/*previousChar = mapData[player.position.x + WIDTH * player.position.y];*/
		setPreviousPosition(player.pointer, player.position);
		//previousPos = (player.position.x + WIDTH * player.position.y);

		player.position.x--;

		if (player.position.x < 0) {
			player.position.x = 0;
		}

		return true;

	}

	if (GetAsyncKeyState(VK_ESCAPE)) {
		isRunning = false;
	}

	if (gameState == BATTLE) {
		//Button to melee attack
		if (GetAsyncKeyState(0x45)) {
			//Attack current enemy
		}
	}


}

void ai() 
{
	for (int i = 0; i < ENEMIESLENGTH; ++i) {
		setPreviousPosition(enemies[i].pointer, enemies[i].position);
		mapData[vectorToFlatArray(enemies[i].positionPrevious, WIDTH)] = ground;
	}

	/*
	Set the previous positon
	Find where enemy wants to go
		-Find player
		-Are they close enough
		-Choose the quickest route to the player
	*/
	float distance = distanceVector2D(enemies[0].position, player.position);

	if ( distance > 1 && distance < 15) {
		gameState = BATTLE;
		if (enemies[0].position.x == player.position.x) {
			if (enemies[0].position.y < player.position.y) {
				enemies[0].position.y++;
			} 
			else 
			{
				enemies[0].position.y--;
			}
		}
		else if (enemies[0].position.y == player.position.y) {
			if (enemies[0].position.x < player.position.x) 
			{
				enemies[0].position.x++;
			}
			else
			{
				enemies[0].position.x--;
			}
		} 
		else if(enemies[0].position.y < player.position.y && enemies[0].position.x < player.position.x)
		{
			enemies[0].position.y++;
			enemies[0].position.x++;
		} 
		else if(enemies[0].position.y > player.position.y && enemies[0].position.x > player.position.x)
		{
			enemies[0].position.y--;
			enemies[0].position.x--;
		}
		else if (enemies[0].position.y > player.position.y && enemies[0].position.x < player.position.x) 
		{
			--enemies[0].position.y;
			++enemies[0].position.x;
		}
		else if (enemies[0].position.y < player.position.y && enemies[0].position.x > player.position.x) 
		{
			++enemies[0].position.y;
			--enemies[0].position.x;
		}
	}
}

void checkPlayerCollison() {
	//If the player and any enemy have the same positon

	//Enemy Collison
	for (int i = 0; i < ENEMIESLENGTH; ++i) {
		if ((player.position.x == enemies[i].position.x) 
			&& (player.position.y == enemies[i].position.y)) {

			//player.character = 'K';
			//enemies[i].character = 'O';

			
			setPosition(player.pointer, player.positionPrevious);
			setPosition(enemies[i].pointer, enemies[i].positionPrevious);
		}
	}

	//Wall Collsion
}

void printPlayerHealth() 
{
	for (int i = 0; i < 10; ++i)
	{
		//uiData[playerHealthUIStart.x * WIDTH * playerHealthUIStart.y + i] = ;
	}
}