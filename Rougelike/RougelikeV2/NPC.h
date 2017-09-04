#pragma once

#include <string>

#include <Windows.h>::WORD
#include "PlayerData.h"
#include "Vector2D.h"


class NPC
{
public:
	NPC();
	NPC(NPC * npc);
	NPC(char c, WORD cc, std::string name, int heal, int atck, int level);
	~NPC();

	//Visual
	std::string name;
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
	bool isTargetingPlayer;

	void setPreviousPosition(NPC *npc, Vector2D pos);
};

struct Drops
{
	int money;
	int exp;
};
