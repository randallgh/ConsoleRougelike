#include <string>

#include "PlayerManager.h"


void PlayerManager::printHealth(char ui[], int width, InfoBox * infoBox)
{
	std::string health = "Player: " + std::to_string(player.health) + "/" + std::to_string(player.maxHealth);
	infoBox->AddInfo(ui, width, 8, health);
}

void PlayerManager::damage(int damage)
{
	player.health -= damage;

	if (player.health < 0)
	{
		//isRunning = false;
		//TODO Game Manager
	}
}


PlayerManager::PlayerManager()
{
	NPC npc ('@', 15, 100, 10, 1);
	player = npc;

	this->player.position = { 0,0 };
}


PlayerManager::~PlayerManager()
{
}
