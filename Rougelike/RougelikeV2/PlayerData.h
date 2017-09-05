#pragma once


/*
PLayer info goes here
Level and exp
number of health potions
Money
*/

struct PlayerData
{
	int gold = 100;

	int exp = 0;
	int expToNextLevel = 10;

	int potions = 3;
	int maxPotions = 3;

	int arrows = 10;
	int maxArrows = 10;
	
	int potionPower = 30;
};