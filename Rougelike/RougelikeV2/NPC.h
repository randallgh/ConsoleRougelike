#pragma once

#include <Windows.h>::WORD
#include "PlayerData.h"
#include "Vector2D.h"

class NPC
{
public:
	NPC();
	NPC(char c, WORD cc, int heal, int atck, int level);
	~NPC();

	//Visual
	char character;
	WORD characterColor;

	//Position
	Vector2D position;
	Vector2D positionPrevious;

	/*-------------Stats--------------*/
	/*TODO
	Implement the proper stats
	Strength
	Constitution
	Dexterity
	Intellegence
	Willpower
	*/

	int health;
	int maxHealth;

	int attack;
	int defense;

	int level;

	bool hasMoved;
	bool isAlive;


	void setPreviousPosition(NPC *npc, Vector2D pos);
};

struct Drops
{
	int money;
	int exp;
};

