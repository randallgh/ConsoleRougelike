#pragma once
#include "NPC.h"
#include "PlayerData.h"
#include "InfoBox.h"
#include "MessageBoxUI.h"

class PlayerManager
{
public:

	//UI
	int width;
	void * ui;
	InfoBox * infoBox;
	MessageBoxUI * messageBox;

	void printHealth();
	void printLevel();
	void printPotions();
	void printExp();

	//Player data
	NPC player;
	PlayerData data;

	void damage(int damage);
	void heal(int health);
	void addExp(int exp);
	void addPotion(int num);

	PlayerManager(char ui[], int width, InfoBox * infoBox, MessageBoxUI * messageBox);
	~PlayerManager();
};


