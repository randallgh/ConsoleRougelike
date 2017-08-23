#pragma once
#include "Vector2D.h"
#include <Windows.h>::WORD

struct Player
{
	char character = '@';
	WORD characterColor = 15;
	Vector2D position = {0,0};
	Vector2D positionPrevious;

	int health;
	int maxHealth = 100;
	int attack = 10;

	bool hasMoved = false;
};