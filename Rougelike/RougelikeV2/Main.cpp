#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <chrono>

#include <iostream>
#include <string>

#include <fstream>


#include "Player.h"

#include "Enemy.h"

#include "Message.h"
#include "MessageBoxUI.h"

#include "InfoBox.h"

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

char ground = '.';//249;

/*-----UI-----*/
char uiData[WIDTH * UIHEIGHT];

Vector2D playerHealthUIStart = { 59, 8 };

#define PLAYERHEALTHLENGTH 9
char healthA[PLAYERHEALTHLENGTH] = {};

//UI Messagebox x: 0 - 49 y: 0 - 8

MessageBoxUI messageBox({0,0});

InfoBox infoBox({ 51,0 });

//  PrintMessage("Player moved right")
//std::string wMes = "Welcome";

//Message welcome(wMes, 8);


#define MESSAGELENGTH 50
/*-----Player-----*/
Player player;

/*-----Enemies-----*/
#define ENEMIESLENGTH 2

Enemy enemies[ENEMIESLENGTH];
Enemy slime = {20, 5, 'S', "Slime", true};

/*----Battle----*/
Enemy *currentEnemy;

/*---------------------------------------------------------------------*/

void render();
bool input();
void checkPlayerCollison();
void ai();

//Player UI
void printPlayerHealth();
void damagePlayer(int damage);
void healPlayer(int health);
void printPotions();
void printLevel();

bool didProc(int prob);


void damageEnemy(int damage);

//UI
void UIrenderCharA(char a[], int length, Vector2D pos);
void printUI();

//Map
void defaultMapToFile();


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

	/*---Turn off Cursor---*/
	
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(wHnd, &cursorInfo);
	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(wHnd, &cursorInfo);

	/*-----------Map Generation----------*/

	for (int i = 0; i < MAPHEIGHT * WIDTH; ++i)
	{
		mapData[i] = ground;
	}

	//UI Generation
	for (int i = 0; i < UIHEIGHT * WIDTH; ++i) 
	{
		uiData[i] = ' ';
	}

	printUI();

	//Add enemies to map
	enemies[0] = copyEnemy(slime);
	enemies[0].position = { 10, 20 };
	enemies[0].pointer = &enemies[0];

	enemies[1] = copyEnemy(slime);
	enemies[1].position = { 20,20 };
	enemies[1].pointer = &enemies[1];

	//Add player to pos
	player.position = { 0, 0 };
	//mapData[player.position.x * player.position.y] = player.character;
	player.pointer = &player;

	//First player health element
	printPlayerHealth();
	printPotions();
	printLevel();

	//Print welcome message
	Message welcome("Welcome!");
	Message test("I'm a bannana!");

	messageBox.messages[0] = welcome;

	messageBox.Clear(uiData, WIDTH, 50);
	messageBox.Add(test);


	for (Enemy element : enemies) {
		mapData[element.position.x + WIDTH * element.position.y] = element.character;
	}

	mapData[player.position.x + WIDTH * player.position.y] = player.character;

	defaultMapToFile();


	/*---------------------------Game Loop--------------------------*/
	while (isRunning) {

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

		mapData[player.position.x + WIDTH * player.position.y] = player.character;

		render();

		Sleep(100);
	}
}



//Takes whatever is in mapData and adds it to the console buffer
void render()
{
	messageBox.Clear(uiData, WIDTH, 50);
	messageBox.Print(uiData, WIDTH, 50);

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
				consoleBuffer[x + WIDTH * y].Attributes = 15 | FOREGROUND_INTENSITY;
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
		player.hasMoved = true;
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
		player.hasMoved = true;
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
		player.hasMoved = true;
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
		player.hasMoved = true;
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
		//Message move("Player moved north.", 20);
		//messageBox.Add(move);
		player.hasMoved = true;
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
		//Message move("Player moved south.", 20);
		//messageBox.Add(move);
		player.hasMoved = true;
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
		//Message move("Player moved east.", 19);
		//messageBox.Add(move)

		player.hasMoved = true;
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
		
		//Message move("Player moved west.", 19);
		//messageBox.Add(move);
		player.hasMoved = true;
		return true;

	}

	player.hasMoved = false;


	//Button to press to recover health: R key
	//Calls a function to heal
	//Removes a potion from the player
	if (GetAsyncKeyState(0x52)) {

		if (player.health < player.maxHealth) 
		{
			if (player.data.potions > 0)
			{
				messageBox.Add(Message("Healed self."));
				//Heal player
				healPlayer(player.data.potionPower);
				--player.data.potions;
				printPotions();
				return true;
			}
			else
			{
				messageBox.Add(Message("You do not have anymore potions."));
			}
		} 
		else
		{
			messageBox.Add(Message("You cannot heal at max health."));
		}
		
	}


	if (GetAsyncKeyState(VK_ESCAPE)) {
		isRunning = false;
	}

	//if (GetAsyncKeyState(VK_OEM_PERIOD)) {
	//	damagePlayer(10);
	//	messageBox.Clear(uiData, WIDTH, 50);
	//}


	if (gameState == BATTLE) {
		//Find the closest enemy
		for (Enemy element : enemies) 
		{
			if (element.isTargetPlayer && element.isAlive) 
			{
				currentEnemy = element.pointer;
			}
		}

		//Button to melee attack
		if (GetAsyncKeyState(0x45)) 
		{
			//Attack current enemy
			if ((*currentEnemy).isAlive)
			{
				int distance = distanceVector2D(player.position, (*currentEnemy).position);
				if (distance <= 2)
				{
					messageBox.Add(Message("Damage enemy."));

					damageEnemy(player.attack);
				}
			}
			return true;
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
	float distance;
	for (int i = 0; i < ENEMIESLENGTH; ++i)
	{
		Vector2D playerPos = player.positionPrevious;
		 distance = distanceVector2D(enemies[i].position, playerPos);
		 
		 if (enemies[i].isAlive) 
		 {
			 if (distance <= 2)
			 {
				 //Damageplayer
				 int damage = enemies[i].attack;

				 std::string damageS = enemies[i].name;
				 damageS.append(" did " + std::to_string(damage) + " damage to player");
				 Message attack(damageS);

				 messageBox.Add(attack);
				 damagePlayer(damage);
			 }
			 else if (distance >= 2 && distance < 15) {
				 gameState = BATTLE;
				 enemies[i].isTargetPlayer = true;
				 //messageBox.Add(Message("Battle start.", 14));
				 if (enemies[i].position.x == playerPos.x) {
					 if (enemies[i].position.y < playerPos.y) {
						 enemies[i].position.y++;
						 enemies[i].hasMoved = true;
					 }
					 else
					 {
						 enemies[i].position.y--;
						 enemies[i].hasMoved = true;
					 }
				 }
				 else if (enemies[i].position.y == playerPos.y) {
					 if (enemies[i].position.x < playerPos.x)
					 {
						 enemies[i].position.x++;
						 enemies[i].hasMoved = true;
					 }
					 else
					 {
						 enemies[i].position.x--;
						 enemies[i].hasMoved = true;
					 }
				 }
				 else if (enemies[i].position.y < playerPos.y && enemies[i].position.x < playerPos.x)
				 {
					 enemies[i].position.y++;
					 enemies[i].position.x++;
					 enemies[i].hasMoved = true;
				 }
				 else if (enemies[i].position.y > playerPos.y && enemies[i].position.x > playerPos.x)
				 {
					 enemies[i].position.y--;
					 enemies[i].position.x--;
					 enemies[i].hasMoved = true;
				 }
				 else if (enemies[i].position.y > playerPos.y && enemies[i].position.x < playerPos.x)
				 {
					 --enemies[i].position.y;
					 ++enemies[i].position.x;
					 enemies[i].hasMoved = true;
				 }
				 else if (enemies[i].position.y < playerPos.y && enemies[i].position.x > playerPos.x)
				 {
					 ++enemies[i].position.y;
					 --enemies[i].position.x;
					 enemies[i].hasMoved = true;
				 }
			 }
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
			
			if (enemies[i].hasMoved) 
			{
				setPosition(enemies[i].pointer, enemies[i].positionPrevious);
			}

			if (player.hasMoved)
			{
				setPosition(player.pointer, player.positionPrevious);
			}
			
			break;
		}
	}

	//Wall Collsion
}

void damagePlayer(int damage) 
{
	player.health -= damage;
	printPlayerHealth();

	if (player.health < 0) 
	{
		isRunning = false;
	}
}

void healPlayer(int health) 
{
	player.health += health;
	if (player.health > player.maxHealth) 
	{
		player.health = player.maxHealth;
	}
	printPotions();
	printPlayerHealth();
}

void UIrenderCharA(char a[], int length, Vector2D pos) 
{
	for (int i = 0; i < length; ++i)
	{
		uiData[pos.x + WIDTH * pos.y + i] = a[i];
	}
}

//Call this only when the player takes damage
void printPlayerHealth() 
{
	std::string health = "Player: " + std::to_string(player.health) + "/" + std::to_string(player.maxHealth);
	infoBox.AddInfo(uiData, WIDTH, 8, health);
}

void printPotions() 
{
	std::string potion = "Potions: " + std::to_string(player.data.potions) + "/" + std::to_string(player.data.maxPotions);
	infoBox.AddInfo(uiData, WIDTH, 7, potion);
}

void printLevel()
{
	std::string level = "Level: " + std::to_string(player.level);
	infoBox.AddInfo(uiData, WIDTH, 0, level);
}

void EnemyDied();
bool areAllEnemiesDead();
void damageEnemy(int damage) 
{
	(*currentEnemy).health -= damage;

	if ((*currentEnemy).health <= 0)
	{
		(*currentEnemy).isAlive = false;
		//Remove enemy body here

		EnemyDied();
		messageBox.Add(Message("Enemy Dead!"));

		if (areAllEnemiesDead()) 
		{
			gameState = OVERWORLD;
			messageBox.Add(Message("All enemies defeated."));
		}
	}
}

void EnemyDied()
{
	//Add function for adding potions
	//Add Item class
	++player.data.potions;
	player.data.exp += 10;
	messageBox.Add(Message("The enemy was defeated!"));

	if (player.data.exp >= 10) 
	{
		++player.level;
		messageBox.Add(Message("Level up!"));
		printLevel();
	}

	printPotions();
}

bool areAllEnemiesDead() 
{
	for (Enemy element : enemies) 
	{
		if (element.isAlive) 
		{
			return false;
		}
	}
}

void defaultMapToFile() 
{
	/*--------------------------Output default map---------------------------*/
	std::string outputMapS;
	std::ofstream outputMap;
	char outputMapCharA[WIDTH * MAPHEIGHT] = { ' ' };

	outputMap.open("defaultMap.txt");

	//Get a string of data
	for (int y = 0; y < MAPHEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			outputMapCharA[x] = mapData[x + WIDTH * ((MAPHEIGHT - 1) - y)];
		}
		outputMap << outputMapCharA << "\n";
	}

	outputMap.close();

	//Add it to the file


	/*-------------------------------------------------------------------------*/
}

void printUI() 
{
	char healthUI[] = "Player: ";
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
}

bool didProc(int prob) 
{
	if ((rand() % 100 + 1) <= prob)
	{
		return true;
	}

	return false;
}