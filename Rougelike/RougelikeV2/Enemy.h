#pragma once
#include "Vector2D.h"
#include <Windows.h>::WORD

struct Enemy
{
	int health;
	int attack;
	char character;

	WORD characterColor;
	Vector2D position;
};

Enemy copyEnemy(Enemy origin);