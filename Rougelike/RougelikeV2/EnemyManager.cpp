#include "EnemyManager.h"

//EnemyManager::EnemyManager()
//{
//
//}


EnemyManager::EnemyManager(char map[], int width, MessageBoxUI * messageBox, PlayerManager * playerManager)
{
	this->map = map;
	this->width = width;
	this->messageBox = messageBox;
	this->playerManager = playerManager;

	//for (int i = 0; i < MAXENEMIES; ++i) 
	//{
	//	this->enemies->name = "TEST";
	//}
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

void EnemyManager::damageCurrentEnemy(int damage)
{
	currentEnemy->health -= damage;
	messageBox->Add(Message("Did " + std::to_string(damage) + " damage to " + currentEnemy->name));


	if (currentEnemy->health <= 0)
	{
		currentEnemy->isAlive = false;
		//Remove enemy body here

		messageBox->Add(Message("The " + currentEnemy->name + " was defeated!"));
		//playerManager->addPotion(1);
		playerManager->addExp(currentEnemy->exp);
		playerManager->printPotions();
		currentEnemy->character = ' ';

		if (areAllDead())
		{
			//gameState = OVERWORLD;
			messageBox->Add(Message("All enemies defeated."));
		}
	}
	else
	{
		messageBox->Add(Message(currentEnemy->name + " HP: " + std::to_string(currentEnemy->health)));
	}
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
		if (element.isAlive) 
		{
			static_cast<char*>(map)[element.position.x + width * element.position.y] = element.character;
		}
	}
}
