#include <string>

#include "PlayerManager.h"


void PlayerManager::printHealth()
{
	std::string health = "Player: " + std::to_string(this->player.health) + "/" + std::to_string(this->player.maxHealth);
	infoBox->AddInfo(static_cast<char*>(ui), width, 8, health);

}

void PlayerManager::printLevel()
{
	std::string level = "Level: " + std::to_string(this->player.level);
	infoBox->AddInfo(static_cast<char*>(ui), width, 1, level);
}

void PlayerManager::printPotions()
{
	std::string potion = "Potions: " + std::to_string(this->data.potions) + "/" + std::to_string(this->data.maxPotions);
	infoBox->AddInfo(static_cast<char*>(ui), width, 7, potion);
}

void PlayerManager::printExp()
{
	std::string exp = "EXP: " + std::to_string(this->data.exp) + "/" + std::to_string(data.expToNextLevel);
	infoBox->AddInfo(static_cast<char*>(ui), width, 0, exp);
}

void PlayerManager::damage(int damage)
{
	player.health -= damage;

	if (player.health < 0)
	{
		//isRunning = false;
		//TODO Game Manager
	}
	//Print player health
	printHealth();
}

void PlayerManager::heal(int health)
{
	this->player.health += health;
	if (this->player.health > this->player.maxHealth)
	{
		this->player.health = this->player.maxHealth;
	}
	//Print health and potions
	printHealth();
	//printPotions
}

void PlayerManager::addExp(int exp)
{
	data.exp += exp;

	if (data.exp >= data.expToNextLevel)
	{
		++player.level;
		messageBox->Add(Message("Level up!"));
		printLevel();
		data.exp -= data.expToNextLevel;

		data.expToNextLevel *= 1.5f;
	}

	printExp();

}

void PlayerManager::addPotion(int num)
{
	data.potions += num;
	if (data.potions > data.maxPotions) 
	{
		data.potions = data.maxPotions;
	}
}


PlayerManager::PlayerManager(char ui[], int width, InfoBox * infoBox, MessageBoxUI * messageBox)
{
	NPC npc ('@', 15, "player", 100, 10, 1);
	this->player = npc;

	this->width = width;
	this->ui = ui;
	this->infoBox = infoBox;
	this->messageBox = messageBox;

	this->player.position = { 0,0 };
}


PlayerManager::~PlayerManager()
{
}
