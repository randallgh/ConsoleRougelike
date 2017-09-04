#pragma once
#include "NPC.h"
#include "PlayerData.h"
#include "InfoBox.h"

class PlayerManager
{
public:

	//UI
	int width;
	void * ui;
	InfoBox * infoBox;

	void printHealth();
	void printLevel();
	void printPotions();

	//Player data
	NPC player;
	PlayerData data;

	void damage(int damage);
	void heal(int health);

	PlayerManager(char ui[], int width, InfoBox * infoBox);
	~PlayerManager();
};


