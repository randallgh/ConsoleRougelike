#pragma once
#include "Vector2D.h"
#include <Windows.h>::WORD

struct Enemy
{
	int health;
	int attack;
	char character;
	char name[10];
	bool isAlive;

	bool hasMoved;
	bool isTargetPlayer;

	WORD characterColor;

	Vector2D position;
	Vector2D positionPrevious;

	Enemy *pointer;
};

Enemy copyEnemy(Enemy origin);
void setPreviousPosition(Enemy *enemy, Vector2D pos);
void setPosition(Enemy *enemy, Vector2D pos);
