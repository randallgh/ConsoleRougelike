#pragma once
#include "NPC.h"
#include "Vector2D.h"
#include "MessageBoxUI.h"
#include "PlayerManager.h"
#include "GameManager.h"
#include "GameObject.h"

#include <string>

class EnemyManager
{
public:

	#define MAXENEMIES 30
	#define DATABASESIZE 100

	//enemies char
	NPC enemies[MAXENEMIES];
	NPC database[DATABASESIZE];

	//Getters
	int getMaxEnemies();

	bool addEnemy(NPC npc, Vector2D pos);
	//database
	void addDatabase(NPC npc);
	NPC getEnemyData(std::string name);

	void printEnemies();
	bool areAllDead();

	void damageCurrentEnemy(int damage);

	void * map;
	int width;

	NPC *currentEnemy;
	MessageBoxUI * messageBox;
	PlayerManager * playerManager;
	GameManager * gameManager;

	EnemyManager(GameObject map[], int width, MessageBoxUI * messageBox, PlayerManager * playerManager, GameManager * gameManager);
	~EnemyManager();
};

