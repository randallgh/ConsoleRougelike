#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <chrono>

#include <iostream>
#include <string>

#include <fstream>


#include "PlayerManager.h"
#include "EnemyManager.h"
//#include "Player.h"
//#include "Enemy.h"

#include "Message.h"
#include "MessageBoxUI.h"

#include "InfoBox.h"

#include "Window.h"

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

char ground = ' ';//249;
char wall = 178;

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
PlayerManager playerM(uiData, WIDTH, &infoBox, &messageBox);
//Player player;

/*-----Enemies-----*/
EnemyManager enemyManager(mapData,WIDTH,&messageBox,&playerM);

//Enemy enemies[ENEMIESLENGTH];
//Enemy slime = {20, 5, 'S', "Slime", true};

/*----Battle----*/


/*---------------------------------------------------------------------*/

void render();
bool input();
void ai();
void physics();


//Random
bool didProc(int prob);

//UI
void printUI();

//Map
void outputMapFromMem(std::string fileName);
void importMapFromFile(std::string fileName);


int main()
{

	srand(time(0));
	initWindow();

	for (int i = 0; i < UIHEIGHT * WIDTH; ++i) 
	{
		uiData[i] = ' ';
	}

	printUI();

	importMapFromFile("home.txt");

	playerM.printHealth();
	playerM.printPotions();
	playerM.printLevel();
	playerM.printExp();

	/*---------------------------Game Loop--------------------------*/
	while (isRunning) {

		switch (gameState)
		{
		case OVERWORLD:
			input();
			mapData[vectorToFlatArray(playerM.player.positionPrevious, WIDTH)] = ground;
			ai();
			physics();
			break;

		case BATTLE:
			if (input()) {
				mapData[vectorToFlatArray(playerM.player.positionPrevious, WIDTH)] = ground;
				ai();
				physics();
			}
			break;
		default:
			break;
		}
		//Add enemies and player to map
		enemyManager.printEnemies();
		mapData[playerM.player.position.x + WIDTH * playerM.player.position.y] = playerM.player.character;
		render();

		Sleep(90);
	}
}

//Takes whatever is in mapData and adds it to the console buffer
void render()
{
	messageBox.Clear(uiData, WIDTH, 50);
	messageBox.Print(uiData, WIDTH, 50);

	char tempChar = 0;
	WORD tempColor = 0;
	int y, x;
	for (y = 0; y < HEIGHT; ++y) 
	{
		for (x = 0; x < WIDTH; ++x) 
		{
			if ((x + WIDTH * ((MAPHEIGHT - 1) - y))  >= 0) {
				tempChar = mapData[x + WIDTH * ((MAPHEIGHT - 1) - y)];

				consoleBuffer[x + WIDTH * y].Char.AsciiChar = tempChar;
				
				if (tempChar == playerM.player.character) 
				{
					 tempColor = playerM.player.characterColor;
				}
				else if (tempChar == ground) 
				{
					tempColor = 10;
				}
				else if (tempChar == wall)
				{
					tempColor = 6;
				}
				else if (tempChar == 'S') 
				{
					tempColor = 11;
				}

				consoleBuffer[x + WIDTH * y].Attributes = tempColor;

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

	Vector2D *playerPos = &playerM.player.position;
	playerM.player.setPreviousPosition((*playerPos));
	//North East
	if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))
	{	
		++(*playerPos).y;
		++(*playerPos).x;

		if ((*playerPos).y >= MAPHEIGHT) {
			(*playerPos).y = (MAPHEIGHT - 1);

		}

		if ((*playerPos).x >= WIDTH) {
			(*playerPos).x = (WIDTH - 1);
		}
		playerM.player.hasMoved = true;
		return true;

	}
	//South East
	else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_RIGHT))
	{
		--(*playerPos).y;
		++(*playerPos).x;
		

		if ((*playerPos).y <= 0) {
			(*playerPos).y = 0;
		}

		if ((*playerPos).x >= WIDTH) {
			(*playerPos).x = (WIDTH - 1);
		}
		playerM.player.hasMoved = true;
		return true;

	}
	//South West
	else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
	{
		--(*playerPos).y;
		--(*playerPos).x;

		if ((*playerPos).y <= 0) {
			(*playerPos).y = 0;
		}

		if ((*playerPos).x < 0) {
			(*playerPos).x = 0;
		}
		playerM.player.hasMoved = true;
		return true;

	}
	//North West
	else if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))
	{
		++(*playerPos).y;
		--(*playerPos).x;

		if ((*playerPos).y >= MAPHEIGHT) {
			(*playerPos).y = (MAPHEIGHT - 1);

		}

		if ((*playerPos).x < 0) {
			(*playerPos).x = 0;   
		}
		playerM.player.hasMoved = true;
		return true;

	}
	//North
	else if (GetAsyncKeyState(VK_UP))
	{
		(*playerPos).y++;

		if ((*playerPos).y >= MAPHEIGHT) {
			(*playerPos).y = (MAPHEIGHT - 1);
		}
		//Message move("playerM.player moved north.", 20);
		//messageBox.Add(move);
		playerM.player.hasMoved = true;
		return true;
	}
	//South
	else if (GetAsyncKeyState(VK_DOWN))
	{
		(*playerPos).y--;

		if ((*playerPos).y <= 0) {
			(*playerPos).y = 0;
		}
		//Message move("Player moved south.", 20);
		//messageBox.Add(move);
		playerM.player.hasMoved = true;
		return true;
	}
	//East
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		(*playerPos).x++;

		if ((*playerPos).x >= WIDTH) {
			(*playerPos).x = (WIDTH - 1);
		}
		//Message move("Player moved east.", 19);
		//messageBox.Add(move)

		playerM.player.hasMoved = true;
		return true;
	}
	//West
	else if (GetAsyncKeyState(VK_LEFT))
	{
		(*playerPos).x--;

		if ((*playerPos).x < 0) {
			(*playerPos).x = 0;
		}
		
		//Message move("Player moved west.", 19);
		//messageBox.Add(move);
		playerM.player.hasMoved = true;
		return true;

	}

	playerM.player.hasMoved = false;


	//Button to press to recover health: R key
	//Calls a function to heal
	//Removes a potion from the player
	if (GetAsyncKeyState(0x52)) //R key
	{

		if (playerM.player.health < playerM.player.maxHealth) 
		{
			if (playerM.data.potions > 0)
			{
				messageBox.Add(Message("Healed self."));
				//Heal player
				playerM.heal(playerM.data.potionPower);
				--playerM.data.potions;
				playerM.printPotions();
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

		for (int i = 0; i < enemyManager.getMaxEnemies(); ++i) 
		{
			if (enemyManager.currentEnemy == nullptr) 
			{
				enemyManager.currentEnemy = &enemyManager.enemies[i];
			}

			NPC * element = &enemyManager.enemies[i];
			if (element->isTargetingPlayer && element->isAlive)
			{
				if (distanceVector2D(element->position, playerM.player.position)
					< distanceVector2D(enemyManager.currentEnemy->position, playerM.player.position)){
					enemyManager.currentEnemy = element;
				}

				if (!enemyManager.currentEnemy->isAlive)
				{
					enemyManager.currentEnemy = element;
				}
			}
		}

		//Button to melee attack
		if (GetAsyncKeyState(0x45)) //E key
		{
			//Attack current enemy
			if ((*enemyManager.currentEnemy).isAlive)
			{
				int distance = distanceVector2D((*playerPos), (*enemyManager.currentEnemy).position);
				if (distance <= 2)
				{
					//messageBox.Add(Message("Damage enemy."));
					enemyManager.damageCurrentEnemy(playerM.player.attack);
				}
			}
			return true;
		}
	}


}

void ai() 
{
	for (int i = 0; i < enemyManager.getMaxEnemies(); ++i) {

		enemyManager.enemies[i].setPreviousPosition(enemyManager.enemies[i].position);
		//setPreviousPosition(enemies[i].pointer, enemies[i].position);
		mapData[vectorToFlatArray(enemyManager.enemies[i].positionPrevious, WIDTH)] = ground;
	}

	/*
	Set the previous positon
	Find where enemy wants to go
		-Find player
		-Are they close enough
		-Choose the quickest route to the player
	*/
	float distance;
	for (int i = 0; i < enemyManager.getMaxEnemies(); ++i)
	{
		Vector2D playerPos = playerM.player.positionPrevious;
		 distance = distanceVector2D(enemyManager.enemies[i].position, playerPos);
		 
		 if (enemyManager.enemies[i].isAlive)
		 {
			 if (distance <= 2)
			 {
				 //Damageplayer
				 int damage = enemyManager.enemies[i].attack;

				 std::string damageS = enemyManager.enemies[i].name + " did " 
									+ std::to_string(damage) + " damage to player";
				 //damageS.append();
				 Message attack(damageS);

				 messageBox.Add(attack);

				 playerM.damage(damage);
				 playerM.printHealth();
			 }
			 else if (distance >= 2 && distance < 15) {
				 gameState = BATTLE;
				 enemyManager.enemies[i].isTargetingPlayer = true;
				 //messageBox.Add(Message("Battle start.", 14));
				 if (enemyManager.enemies[i].position.x == playerPos.x) {
					 if (enemyManager.enemies[i].position.y < playerPos.y) {
						 enemyManager.enemies[i].position.y++;
						 enemyManager.enemies[i].hasMoved = true;
					 }
					 else
					 {
						 enemyManager.enemies[i].position.y--;
						 enemyManager.enemies[i].hasMoved = true;
					 }
				 }
				 else if (enemyManager.enemies[i].position.y == playerPos.y) {
					 if (enemyManager.enemies[i].position.x < playerPos.x)
					 {
						 enemyManager.enemies[i].position.x++;
						 enemyManager.enemies[i].hasMoved = true;
					 }
					 else
					 {
						 enemyManager.enemies[i].position.x--;
						 enemyManager.enemies[i].hasMoved = true;
					 }
				 }
				 else if (enemyManager.enemies[i].position.y < playerPos.y && enemyManager.enemies[i].position.x < playerPos.x)
				 {
					 enemyManager.enemies[i].position.y++;
					 enemyManager.enemies[i].position.x++;
					 enemyManager.enemies[i].hasMoved = true;
				 }
				 else if (enemyManager.enemies[i].position.y > playerPos.y && enemyManager.enemies[i].position.x > playerPos.x)
				 {
					 enemyManager.enemies[i].position.y--;
					 enemyManager.enemies[i].position.x--;
					 enemyManager.enemies[i].hasMoved = true;
				 }
				 else if (enemyManager.enemies[i].position.y > playerPos.y && enemyManager.enemies[i].position.x < playerPos.x)
				 {
					 --enemyManager.enemies[i].position.y;
					 ++enemyManager.enemies[i].position.x;
					 enemyManager.enemies[i].hasMoved = true;
				 }
				 else if (enemyManager.enemies[i].position.y < playerPos.y && enemyManager.enemies[i].position.x > playerPos.x)
				 {
					 ++enemyManager.enemies[i].position.y;
					 --enemyManager.enemies[i].position.x;
					 enemyManager.enemies[i].hasMoved = true;
				 }
			 }
		 }
		 if (enemyManager.enemies[i].position.x == enemyManager.enemies[i].positionPrevious.x
			 && enemyManager.enemies[i].position.y == enemyManager.enemies[i].positionPrevious.y)
		 {
			 enemyManager.enemies[i].hasMoved = false;
		 }
	}




}

void physics() {
	//If the player and any enemy have the same positon

	//Enemy Collison
	for (int i = 0; i < enemyManager.getMaxEnemies(); ++i) {
		
		if (isSameVectors(playerM.player.position, enemyManager.enemies[i].position)
			&& enemyManager.enemies[i].isAlive) 
		{

			//player.character = 'K';
			//enemies[i].character = 'O';
			
			if (enemyManager.enemies[i].hasMoved)
			{
				enemyManager.enemies[i].setPosition(enemyManager.enemies[i].positionPrevious);
			}

			if (playerM.player.hasMoved)
			{
				playerM.player.setPosition(playerM.player.positionPrevious);
			}
		}
	}

	//Wall Collsion

	for (int i = 0; i < enemyManager.getMaxEnemies(); ++i) 
	{
		//If the new position is on a wall send the enemy back a space
		//Implement walls as a object and refer to those positions rather then map data
		int pos = enemyManager.enemies[i].position.x + WIDTH * enemyManager.enemies[i].position.y;
		if (mapData[pos] == wall) 
		{
			enemyManager.enemies[i].setPosition(enemyManager.enemies[i].positionPrevious);
		}
	}

	int playerPos = playerM.player.position.x + WIDTH * playerM.player.position.y;

	if (mapData[playerPos] == wall) 
	{
		playerM.player.setPosition(playerM.player.positionPrevious);
	}

	//If the player is on a wall send them back
}

void importMapFromFile(std::string fileName) 
{
	std::ifstream input;
	std::string inputString;

	input.open(fileName);

	if (input.is_open()) 
	{
		int y = 0;
		while (getline(input, inputString)) 
		{
			for (int x = 0; x < WIDTH; ++x) 
			{
				mapData[x + WIDTH * ((MAPHEIGHT - 1) - y)] = inputString.at(x);
			}

			++y;
		}
	}

	/*
	Loop through each char looking for
	W - wall
	@ - set player position
	S - add a enemy at that position
	*/

	NPC slime('S', 15, "Slime", 20, 5, 1);

	for (int y = 0; y < MAPHEIGHT; ++y) 
	{
		for (int x = 0; x < WIDTH; ++x) 
		{
			switch (mapData[x + WIDTH * y])
			{
			case '@':
				playerM.player.position = { x,y };
				break;
			case 'S':
				//messageBox.Add(Message("Add enemy"));
				enemyManager.addEnemy(slime, { x,y });
				break;
			case 'w':
				mapData[x + WIDTH * y] = wall;
				break;
			case '.':
				mapData[x + WIDTH * y] = ' ';
			default:
				break;
			}
		}
	}

	input.close();
	
}

void outputMapFromMem(std::string fileName) 
{
	std::string outputMapString;
	std::ofstream output;
	char outputCharA[WIDTH * MAPHEIGHT] = { ' ' };

	output.open(fileName);

	for (int y = 0; y < MAPHEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			outputCharA[x] = mapData[x + WIDTH * ((MAPHEIGHT - 1) - y)];
		}
		output << outputCharA << "\n";
	}

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