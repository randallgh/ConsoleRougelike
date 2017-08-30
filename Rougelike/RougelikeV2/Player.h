#pragma once
#include <Windows.h>::WORD

#include "PlayerData.h"
#include "Vector2D.h"


struct Player
{
	char character = '@';
	WORD characterColor = 15;
	Vector2D position = { 0,0 };
	Vector2D positionPrevious = { 0,0 };

	int health = 100;
	int maxHealth = 100;
	int attack = 10;

	int level = 1;

	bool hasMoved = false;
	bool isAlive = true;

	PlayerData data;
	Player *pointer;
};

void setPosition(Player *player, Vector2D pos);
void setPreviousPosition(Player *player, Vector2D pos);