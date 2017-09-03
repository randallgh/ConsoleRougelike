#pragma once
#include "NPC.h"
#include "PlayerData.h"
#include "InfoBox.h"

class PlayerManager
{
public:

	int width;
	void * ui;
	InfoBox * infoBox;

	NPC player;
	PlayerData data;

	void printHealth();
	void printLevel();
	void printPotions();

	void damage(int damage);
	void heal(int health);

	PlayerManager(char ui[], int width, InfoBox * infoBox);
	~PlayerManager();
};


