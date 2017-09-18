#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <chrono>

#include <iostream>
#include <string>

#include <fstream>


#include "PlayerManager.h"
#include "EnemyManager.h"
#include "GameManager.h"
//#include "Player.h"
//#include "Enemy.h"

#include "Message.h"
#include "MessageBoxUI.h"

#include "GameObject.h"

#include "InfoBox.h"

#include "Window.h"

/*-------------------------------DATA----------------------------------*/
bool isRunning = true;

/*-----Map-----*/
//char mapData[WIDTH * MAPHEIGHT];
GameObject * levelData = new GameObject[WIN_WIDTH * MAP_HEIGHT];

const char ground = ' ';//249;
const char wall = 178;

int level = 1;
std::string currentMap = "level1";

/*-----UI-----*/
char * uiData = new char[WIN_WIDTH * UI_HEIGHT];

Vector2D playerHealthUIStart = { 59, 8 };

#define PLAYERHEALTHLENGTH 9
char healthA[PLAYERHEALTHLENGTH] = {};

//UI Messagebox x: 0 - 49 y: 0 - 8

MessageBoxUI messageBox({0,0});

InfoBox infoBox({ 51,0 });
#define MESSAGELENGTH 50

GameManager gameManager;
/*-----Player-----*/
PlayerManager playerManager(uiData, WIN_WIDTH, &infoBox, &messageBox);
//Player player;

/*-----Enemies-----*/
EnemyManager enemyManager(levelData,WIN_WIDTH,&messageBox,&playerManager,&gameManager);

/*---------------------------------------------------------------------*/

void render();
bool input();
void ai();
void physics();


//Random
bool didProc(int prob);
void loadEnemies();

//UI
void printUI();

//Map
void outputMapFromMem(std::string fileName);
void importMapFromFile(std::string fileName);
void savePlayerData(std::string fileName);
void loadPlayerData();


void help() 
{
	messageBox.Add(Message("Welcome to Rougelike!"));
	messageBox.Add(Message("Use the arrow keys to move."));
	messageBox.Add(Message("E to attack. R to use bow. H to restore health."));
	messageBox.Add(Message("Head to the > to challange the dragon."));
	messageBox.Add(Message("Press the I key to see this imformation again."));
}

int main()
{

	srand(time(0));
	initWindow();


	for (int i = 0; i < UI_HEIGHT * WIN_WIDTH; ++i) 
	{
		uiData[i] = ' ';
	}

	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < WIN_WIDTH; ++x) {
			levelData[x + WIN_WIDTH * y].position = { x,y };
		}
	}
	loadPlayerData();

	printUI();
	loadEnemies();
	importMapFromFile(currentMap);
	//importMapFromFile("maps/map.txt");
	playerManager.data.arrows = 10;

	playerManager.printHealth();
	playerManager.printPotions();
	playerManager.printLevel();
	playerManager.printExp();
	playerManager.printPotionPower();
	playerManager.printArrows();

	help();

	/*---------------------------Game Loop--------------------------*/
	while (isRunning) {

		switch (gameManager.gameState)
		{
		case gameManager.OVERWORLD:
			input();
			levelData[vectorToFlatArray(playerManager.player.positionPrevious, WIN_WIDTH)].character = ground;
			ai();
			physics();
			break;

		case gameManager.BATTLE:
			if (input()) {
				levelData[vectorToFlatArray(playerManager.player.positionPrevious, WIN_WIDTH)].character = ground;
				ai();
				physics();
			}
			break;
		case gameManager.DEAD:
			if (GetAsyncKeyState(VK_ESCAPE)) {
				isRunning = false;
			}
			break;
		default:
			break;
		}
		//Add enemies and player to map
		enemyManager.printEnemies();
		levelData[playerManager.player.position.x + WIN_WIDTH * playerManager.player.position.y].character = playerManager.player.character;
		levelData[playerManager.player.position.x + WIN_WIDTH * playerManager.player.position.y].color = playerManager.player.color;
		render();

		Sleep(100);
	}

	if (playerManager.player.isAlive) 
	{
		outputMapFromMem("data/maps/mapSave.txt");
		savePlayerData("saves/playerSave.txt");
	}

	delete[] levelData;
	delete[] uiData;

	return 0;
}

//Takes whatever is in mapData and adds it to the console buffer
void render()
{
	messageBox.Clear(uiData, WIN_WIDTH, 50);
	messageBox.Print(uiData, WIN_WIDTH, 50);

	GameObject tempObject;

	int y, x;
	for (y = 0; y < WIN_HEIGHT; ++y) 
	{
		for (x = 0; x < WIN_WIDTH; ++x) 
		{
			if ((x + WIN_WIDTH * ((MAP_HEIGHT - 1) - y))  >= 0) {

				tempObject = levelData[x + WIN_WIDTH * ((MAP_HEIGHT - 1) - y)];

				consoleBuffer[x + WIN_WIDTH * y].Char.AsciiChar = tempObject.character;
				consoleBuffer[x + WIN_WIDTH * y].Attributes = tempObject.color;
				
			} 
			if ((x + WIN_WIDTH  * y) >= MAP_HEIGHT * WIN_WIDTH ) {
				consoleBuffer[x + WIN_WIDTH * y].Char.AsciiChar = uiData[x + WIN_WIDTH * ((UI_HEIGHT - 1) - (y - MAP_HEIGHT))];
				consoleBuffer[x + WIN_WIDTH * y].Attributes = 15 | FOREGROUND_INTENSITY;
			}
		}
	}

	/* Write our character buffer to the console buffer */
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}

bool input() {

	Vector2D *playerPos = &playerManager.player.position;
	playerManager.player.setPreviousPosition((*playerPos));

	//H Key
	if (GetAsyncKeyState(0x48))
	{

		if (playerManager.player.health < playerManager.player.maxHealth)
		{
			if (playerManager.data.potions > 0)
			{
				messageBox.Add(Message("Healed self."));
				//Heal player
				playerManager.heal(playerManager.data.potionPower);
				--playerManager.data.potions;
				playerManager.printPotions();
				return true;
			}
			else
			{
				messageBox.Add(Message("You do not have any potions."));
			}
		}
		else
		{
			messageBox.Add(Message("You cannot heal at max health."));
		}

	}


	if (gameManager.gameState == gameManager.BATTLE) {
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
				if (distanceVector2D(element->position, playerManager.player.position)
					< distanceVector2D(enemyManager.currentEnemy->position, playerManager.player.position)) {
					enemyManager.currentEnemy = element;
				}

				if (!enemyManager.currentEnemy->isAlive)
				{
					enemyManager.currentEnemy = element;
				}
			}
		}

		//E key - Melee

		if (GetAsyncKeyState(0x45))
		{
			//Attack current enemy
			if ((*enemyManager.currentEnemy).isAlive)
			{
				int distance = distanceVector2D((*playerPos), (*enemyManager.currentEnemy).position);
				if (distance <= 1)
				{
					//messageBox.Add(Message("Damage enemy."));
					if (didProc(playerManager.player.dexterity))
					{
						enemyManager.damageCurrentEnemy(playerManager.player.attack);
					}
					else
					{
						messageBox.Add(Message("Missed attack!"));
					}

				}
			}
			return true;
		}

		//R key - Ranged

		if (GetAsyncKeyState(0x52)) 
		{
			if ((*enemyManager.currentEnemy).isAlive)
			{
				int distance = distanceVector2D((*playerPos), (*enemyManager.currentEnemy).position);
				if (playerManager.data.arrows <= 0) 
				{
					messageBox.Add(Message("No arrows left!"));
				}
				else if (distance <= 10)
				{
					if (didProc(playerManager.player.dexterity))
					{
						messageBox.Add(Message("Player shot bow."));
						enemyManager.damageCurrentEnemy(playerManager.player.attack);
					}
					else
					{
						messageBox.Add(Message("Missed ranged attack!"));
					}
					--playerManager.data.arrows;
					playerManager.printArrows();
					return true;
				}
			}
		}
	}

	//North East
	if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))
	{	
		++(*playerPos).y;
		++(*playerPos).x;

		if ((*playerPos).y >= MAP_HEIGHT) {
			(*playerPos).y = (MAP_HEIGHT - 1);

		}

		if ((*playerPos).x >= WIN_WIDTH) {
			(*playerPos).x = (WIN_WIDTH - 1);
		}
		playerManager.player.hasMoved = true;
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

		if ((*playerPos).x >= WIN_WIDTH) {
			(*playerPos).x = (WIN_WIDTH - 1);
		}
		playerManager.player.hasMoved = true;
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
		playerManager.player.hasMoved = true;
		return true;

	}
	//North West
	else if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))
	{
		++(*playerPos).y;
		--(*playerPos).x;

		if ((*playerPos).y >= MAP_HEIGHT) {
			(*playerPos).y = (MAP_HEIGHT - 1);

		}

		if ((*playerPos).x < 0) {
			(*playerPos).x = 0;   
		}
		playerManager.player.hasMoved = true;
		return true;

	}
	//North
	else if (GetAsyncKeyState(VK_UP))
	{
		(*playerPos).y++;

		if ((*playerPos).y >= MAP_HEIGHT) {
			(*playerPos).y = (MAP_HEIGHT - 1);
		}
		//Message move("playerM.player moved north.", 20);
		//messageBox.Add(move);
		playerManager.player.hasMoved = true;
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
		playerManager.player.hasMoved = true;
		return true;
	}
	//East
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		(*playerPos).x++;

		if ((*playerPos).x >= WIN_WIDTH) {
			(*playerPos).x = (WIN_WIDTH - 1);
		}
		//Message move("Player moved east.", 19);
		//messageBox.Add(move)

		playerManager.player.hasMoved = true;
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
		playerManager.player.hasMoved = true;
		return true;

	}

	playerManager.player.hasMoved = false;


	//Button to press to recover health: R key
	//Calls a function to heal
	//Removes a potion from the player

	//I for info
	if (GetAsyncKeyState(0x49)) {
		help();
	}

	//Debug Key Period
	if (GetAsyncKeyState(VK_OEM_PERIOD)) {
		importMapFromFile("test");
	}

	//if (GetAsyncKeyState(VK_OEM_COMMA)) {
	//	importMapFromFile("maps/level2.txt");
	//}

	//Escape
	if (GetAsyncKeyState(VK_ESCAPE)) {
		isRunning = false;
	}

	//if (GetAsyncKeyState(VK_OEM_PERIOD)) {
	//	damagePlayer(10);
	//	messageBox.Clear(uiData, WIDTH, 50);
	//}

}

void ai() 
{
	for (int i = 0; i < enemyManager.getMaxEnemies(); ++i) {

		enemyManager.enemies[i].setPreviousPosition(enemyManager.enemies[i].position);
		//setPreviousPosition(enemies[i].pointer, enemies[i].position);
		levelData[vectorToFlatArray(enemyManager.enemies[i].positionPrevious, WIN_WIDTH)].character = ground;
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
		Vector2D playerPos = playerManager.player.positionPrevious;
		 distance = distanceVector2D(enemyManager.enemies[i].position, playerPos);
		 
		 if (enemyManager.enemies[i].isAlive)
		 {
			 if (distance <= 1)
			 {
				 //Damageplayer
				 if (didProc(enemyManager.enemies[i].dexterity)) 
				 {
					 int damage = enemyManager
						 .enemies[i].attack;

					 std::string damageS = enemyManager.enemies[i].name + " did "
						 + std::to_string(damage) + " damage to player";
					 //damageS.append();
					 Message attack(damageS);

					 messageBox.Add(attack);

					 playerManager.damage(damage);
					 playerManager.printHealth();
					 if (playerManager.player.health <= 0) 
					 {
						 playerManager.player.isAlive = false;
						 gameManager.gameState = gameManager.DEAD;
						 messageBox.Add(Message(" "));
						 messageBox.Add(Message("You have died! Hit ESC to end the game."));
						 messageBox.Add(Message("Relaunch to try again."));
					 }
				 } 
				 else 
				 {
					 messageBox.Add(Message(enemyManager.enemies[i].name + " missed."));
				 }
			 }
			 else if (distance >= 1 && distance < 15) {
				 gameManager.gameState = gameManager.BATTLE;
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
		
		if (isSameVectors(playerManager.player.position, enemyManager.enemies[i].position)
			&& enemyManager.enemies[i].isAlive) 
		{

			//player.character = 'K';
			//enemies[i].character = 'O';
			if (enemyManager.enemies[i].hasMoved && playerManager.player.hasMoved) 
			{
				enemyManager.enemies[i].setPosition(enemyManager.enemies[i].positionPrevious);
			}
			else
			{
				if (enemyManager.enemies[i].hasMoved)
				{
					enemyManager.enemies[i].setPosition(enemyManager.enemies[i].positionPrevious);
				}

				if (playerManager.player.hasMoved)
				{
					playerManager.player.setPosition(playerManager.player.positionPrevious);
				}
			}

			/*for (int e = 0; e < enemyManager.getMaxEnemies(); ++e) 
			{
				if (isSameVectors(enemyManager.enemies[i].position, enemyManager.enemies[e].position)
					&& enemyManager.enemies[i].isAlive && enemyManager.enemies[e].isAlive)
				{
					messageBox.Add(Message("ENEMY ENEMY COLLISION");
					if (enemyManager.enemies[i].hasMoved && enemyManager.enemies[e].hasMoved)
					{
						enemyManager.enemies[i].setPosition(enemyManager.enemies[i].positionPrevious);
					}
					else
					{
						if (enemyManager.enemies[i].hasMoved)
						{
							enemyManager.enemies[i].setPosition(enemyManager.enemies[i].positionPrevious);
						}

						if (enemyManager.enemies[e].hasMoved)
						{
							enemyManager.enemies[e].setPosition(enemyManager.enemies[e].positionPrevious);
						}
					}

					if(!(&enemyManager.enemies[i] == &enemyManager.enemies[e]))
					{
					
					}
				}
			}*/
			//playerManager.damage(enemyManager.enemies[i].attack);
			//enemyManager.damageCurrentEnemy(playerManager.player.attack);
		}
	}

	//Wall Collsion

	for (int i = 0; i < enemyManager.getMaxEnemies(); ++i) 
	{
		//If the new position is on a wall send the enemy back a space
		//Implement walls as a object and refer to those positions rather then map data
		int pos = enemyManager.enemies[i].position.x + WIN_WIDTH * enemyManager.enemies[i].position.y;
		if (levelData[pos].character == wall) 
		{
			enemyManager.enemies[i].setPosition(enemyManager.enemies[i].positionPrevious);
		}
	}

	int playerPos = playerManager.player.position.x + WIN_WIDTH * playerManager.player.position.y;

	if (levelData[playerPos].character == wall) 
	{
		playerManager.player.setPosition(playerManager.player.positionPrevious);
	}
	if (levelData[playerPos].character == 'p') 
	{
		bool isTrue = playerManager.addPotion(1);
		if (!isTrue) 
		{
			playerManager.player.setPosition(playerManager.player.positionPrevious);
		}
	}
	if (levelData[playerPos].character == 'a')
	{
		bool isTrue = playerManager.addArrow(10);
		if (!isTrue)
		{
			playerManager.player.setPosition(playerManager.player.positionPrevious);
		}
	}
	if (levelData[playerPos].character == '>')
	{
		++level;
		importMapFromFile("level" + std::to_string(level));
	}

	//If the player is on a wall send them back
}

void importMapFromFile(std::string fileName) 
{
	std::string destination = "data/maps/" + fileName + ".txt";

	for (int i = 0; i < enemyManager.getMaxEnemies(); ++i)
	{
		enemyManager.enemies[i].isAlive = false;
		enemyManager.enemies[i].health = 0;
		enemyManager.enemies[i].character = ' ';
	}

	for (int i = 0; i < MAP_HEIGHT * WIN_WIDTH; ++i)

	{
		levelData[i].character = ground;
		levelData[i].color = 15;
	}

	std::ifstream input;
	std::string inputString;

	input.open(destination);

	if (input.is_open()) 
	{
		int y = 0;
		while (getline(input, inputString)) 
		{
			for (int x = 0; x < WIN_WIDTH; ++x) 
			{
				inputString.resize(WIN_WIDTH);
				levelData[x + WIN_WIDTH * ((MAP_HEIGHT - 1) - y)].character = inputString.at(x);
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

	for (int y = 0; y < MAP_HEIGHT; ++y) 
	{
		for (int x = 0; x < WIN_WIDTH; ++x) 
		{
			switch (levelData[x + WIN_WIDTH * y].character)
			{
			case '@':
				playerManager.player.position = { x,y };
				break;
			case 'S':
				//messageBox.Add(Message("Add enemy"));
				enemyManager.addEnemy(enemyManager.getEnemyData("Blue Slime"), { x,y });

				break;
			case 'D':
				enemyManager.addEnemy(enemyManager.getEnemyData("Red Dragon"), { x,y });
				break;
			case 'w':
				levelData[x + WIN_WIDTH * y].character = wall;
				levelData[x + WIN_WIDTH * y].color = 6;
				break;
			case '.':
				levelData[x + WIN_WIDTH * y].character = ground;
			case 'p':
				levelData[x + WIN_WIDTH * y].color = 13;
				break;
			case 'a':
				levelData[x + WIN_WIDTH * y].color = 14;
				break;
			default:
				break;
			}
		}
	}

	input.close();
}

void outputMapFromMem(std::string fileName) 
{
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < WIN_WIDTH; ++x)
		{
			switch (levelData[x + WIN_WIDTH * y].character)
			{
			case wall:
				levelData[x + WIN_WIDTH * y].character = 'w';
				break;
			case ground:
				levelData[x + WIN_WIDTH * y].character = '.';
				break;
			}
		}
	}

	std::string outputMapString;
	std::ofstream output;
	char outputCharA[WIN_WIDTH * MAP_HEIGHT] = { ' ' };

	output.open(fileName);

	for (int y = 0; y < MAP_HEIGHT; ++y) {
		for (int x = 0; x < WIN_WIDTH; ++x) {
			outputCharA[x] = levelData[x + WIN_WIDTH * ((MAP_HEIGHT - 1) - y)].character;
		}
		output << outputCharA << "\n";
	}
	output.flush();
	output.close();
}

void savePlayerData(std::string fileName)
{
	std::ofstream file;

	file.open(fileName);


	file << playerManager.player.isAlive << "\n";
	file << level << "\n";
	//Player Data
	file << playerManager.data.exp << "\n";
	file << playerManager.data.expToNextLevel << "\n";

	file << playerManager.data.arrows << "\n";
	file << playerManager.data.potions << "\n";

	//Player NPC
	file << playerManager.player.health << "\n";
	file << playerManager.player.maxHealth << "\n";
	file << playerManager.player.attack << "\n";
	//file << playerManager.player.defense << "\n";
	file << playerManager.player.level << "\n";
	//Pos
	file << playerManager.player.position.x << "\n";
	file << playerManager.player.position.y << "\n";


	file.flush();
	file.close();

}

void loadPlayerData()
{
	std::ifstream player;
	std::string buffer;
	player.open("saves/playerSave.txt");

	/*	file << playerManager.player.isAlive << "\n";
	//Player Data
	file << playerManager.data.exp << "\n";
	file << playerManager.data.expToNextLevel << "\n";

	file << playerManager.data.arrows << "\n";
	file << playerManager.data.potions << "\n";

	//Player NPC
	file << playerManager.player.health << "\n";
	file << playerManager.player.maxHealth << "\n";
	file << playerManager.player.attack << "\n";
	//file << playerManager.player.defense << "\n";
	file << playerManager.player.level << "\n";
	//Pos
	file << playerManager.player.position.x << "\n";
	file << playerManager.player.position.y << "\n";*/


	if (!player.fail())
	{
		getline(player, buffer);
		if (std::stoi(buffer) == 1)
		{
			getline(player, buffer);
			level = std::stoi(buffer);
			getline(player, buffer);
			int exp = std::stoi(buffer);
			getline(player, buffer);
			int expToNextLevel = std::stoi(buffer);

			getline(player, buffer);
			int arrows = std::stoi(buffer);
			getline(player, buffer);
			int potions = std::stoi(buffer);

			playerManager.data.exp = exp;
			playerManager.data.expToNextLevel = expToNextLevel;
			playerManager.data.arrows = arrows;
			playerManager.data.potions = potions;

			getline(player, buffer);
			int health = std::stoi(buffer);
			getline(player, buffer);
			int maxHealth = std::stoi(buffer);
			getline(player, buffer);
			int attack = std::stoi(buffer);
			getline(player, buffer);
			int level = std::stoi(buffer);

			playerManager.player.attack = attack;
			playerManager.player.health = health;
			playerManager.player.maxHealth = maxHealth;
			playerManager.player.level = level;

			currentMap = "mapSave";

		}
	}
	player.close();
}

void printUI() 
{
	char healthUI[] = "Player: ";
	//Need a line of characters at the top of the UI box
	for (int y = 0; y < UI_HEIGHT; ++y) {
		for (int x = 0; x < WIN_WIDTH; ++x) {
			if (y == 9) {
				uiData[x + WIN_WIDTH * y] = 205;
			}
			if ((x >= 0 && y >= 8) && (x <= 9 && y <= 8)) {
				uiData[x + WIN_WIDTH * y] = x + 48;
			}
			if (x == 50) {
				uiData[x + WIN_WIDTH * y] = 186;
			}
			if (x == 50 && y == 9) {
				uiData[x + WIN_WIDTH * y] = 203;
			}
			if (x == 51 && y == 8) {
				for (int i = 0; i < 8; ++i)
				{
					uiData[x + WIN_WIDTH * y + i] = healthUI[i];
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

void loadEnemies()
{
	//Replace this line with reading in enemy info in from file
	//NPC slime('S', 11, "Blue Slime", 20, 10, 80, 1, 30);
	//NPC greenSlime('S', 10, "Green Slime", 20, 5, 80, 1, 15);
	//NPC dragon('D', 12, "Dragon", 300, 10, 80, 5, 200);
	//enemyManager.addDatabase(slime);
	//enemyManager.addDatabase(greenSlime);
	//enemyManager.addDatabase(dragon);

	std::string enemyFile = "data/enemies.txt";
	std::fstream file;

	file.open(enemyFile);
	if (file.fail())
	{
		messageBox.Add(Message("ENEMY FILE NOT FOUND"));
		return;
	}

	std::string name = "null";
	char c = 'c';
	WORD color = 15;
	int health = 0;
	int attack = 0;
	int dexterity = 0;
	int level = 0;
	int exp = 0;


	std::string buffer = "null";
	while (getline(file, buffer))
	{
		if (buffer.size() > 0) 
		{
			if (buffer.at(0) == '@')
			{
				getline(file, buffer);
				name = buffer;

				getline(file, buffer);
				if (buffer.compare("BLUE") == 0)
				{
					color = 11;
				}
				else if (buffer.compare("RED") == 0)
				{
					color = 12;
				}
				else if (buffer.compare("GREEN") == 0)
				{
					color = 10;
				}

				getline(file, buffer);
				c = buffer.at(0);

				getline(file, buffer);
				health = std::stoi(buffer);

				getline(file, buffer);
				attack = std::stoi(buffer);

				getline(file, buffer);
				dexterity = std::stoi(buffer);

				getline(file, buffer);
				level = std::stoi(buffer);

				getline(file, buffer);
				exp = std::stoi(buffer);

				messageBox.Add(Message("Loaded " + name + " C: " + c));
				enemyManager.addDatabase(new NPC(c, color, name, health, attack, dexterity, level, exp));
			}
		}
	}

	file.close();
}