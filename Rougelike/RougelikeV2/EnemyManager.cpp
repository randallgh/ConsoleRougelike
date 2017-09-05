#include "EnemyManager.h"

//EnemyManager::EnemyManager()
//{
//
//}


EnemyManager::EnemyManager(char map[], int width)
{
	this->map = map;
	this->width = width;


	for (int i = 0; i < MAXENEMIES; ++i) 
	{
		this->enemies->name = "TEST";
	}
}

bool EnemyManager::areAllDead()
{
	for (int i = 0; i < MAXENEMIES; ++i)
	{
		if (enemies[i].isAlive)
		{
			return false;
		}
	}

	return true;
}

EnemyManager::~EnemyManager()
{

}

int EnemyManager::getMaxEnemies()
{
	return (int)MAXENEMIES;
}

bool EnemyManager::addEnemy(NPC npc, Vector2D pos)
{

	for (int i = 0; i < MAXENEMIES; ++i)
	{

		if (this->enemies[i].isAlive == false)
		{
			this->enemies[i] = new NPC(npc);
			this->enemies[i].position = pos;
			this->enemies[i].isAlive = true;
			this->enemies[i].isTargetingPlayer = false;
			return true;
		}
	}
	return false;
}

void EnemyManager::printEnemies()
{
	for (NPC element : this->enemies) {
		static_cast<char*>(map)[element.position.x + width * element.position.y] = element.character;
	}
}
