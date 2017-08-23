#pragma once

#include "Point2D.h"

struct Player
{
	bool isAlive = true;

	int health = 100;
	int attack = 10;

	char character = '@';

	Point2D position = { 0,0 };
};