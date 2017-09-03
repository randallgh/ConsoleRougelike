#pragma once
#include "NPC.h"
#include "PlayerData.h"
#include "InfoBox.h"

class PlayerManager
{
public:

	NPC player;
	PlayerData data;
	


	void printHealth(char ui[], int width, InfoBox * infoBox);
	void damage(int damage);

	PlayerManager();
	~PlayerManager();
};


