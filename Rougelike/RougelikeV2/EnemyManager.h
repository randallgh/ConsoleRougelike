#pragma once
#include "NPC.h"
#include "Vector2D.h"
#include "MessageBoxUI.h"
#include "PlayerManager.h"

class EnemyManager
{
public:

	#define MAXENEMIES 10

	//enemies char
	NPC enemies[MAXENEMIES];

	//Getters
	int getMaxEnemies();

	bool addEnemy(NPC npc, Vector2D pos);
	//TODO
	//void addEnemyToDatabase(NPC npc);
	void printEnemies();
	bool areAllDead();

	void damageCurrentEnemy(int damage);

	void * map;
	int width;

	NPC *currentEnemy;
	MessageBoxUI * messageBox;
	PlayerManager * playerManager;

	EnemyManager(char map[], int width, MessageBoxUI * messageBox, PlayerManager * playerManager);
	~EnemyManager();
};

