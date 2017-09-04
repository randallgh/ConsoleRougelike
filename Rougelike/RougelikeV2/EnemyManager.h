#pragma once
#include "NPC.h"
#include "Vector2D.h"

class EnemyManager
{
public:

	#define MAXENEMIES 2

	//enemies char
	NPC enemies[MAXENEMIES];


	int getMaxEnemies();

	bool addEnemy(NPC npc, Vector2D pos);
	//void addEnemyToDatabase(NPC npc);

	void printEnemies();

	void * map;
	int width;
	EnemyManager(char map[], int width);

	~EnemyManager();
};

