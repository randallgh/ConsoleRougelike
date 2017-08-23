#pragma once
#include "Vector2D.h"

struct Player
{
	char character = '@';
	Vector2D position = {0,0};

	int health;
	int attack;

	bool hasMoved = false;
};