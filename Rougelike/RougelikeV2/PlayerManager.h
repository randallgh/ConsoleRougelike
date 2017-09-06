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
	void printPotionPower();
	void printArrows();

	//Player data
	NPC player;
	PlayerData data;

	void damage(int damage);
	void heal(int health);
	void addExp(int exp);
	bool addPotion(int num);
	bool addArrow(int num);

	PlayerManager(char ui[], int width, InfoBox * infoBox, MessageBoxUI * messageBox);
	~PlayerManager();
};


