#include "NPC.h"



NPC::NPC()
{
}


NPC::NPC(char c, WORD cc, int heal, int atck, int level)
{
	this->character = c;
	this->characterColor = cc;

	this->health = heal;
	this->maxHealth = heal;

	this->attack = atck;
	this->level = level;

	this->isAlive = true;

	this->position = { 0,0 };
	this->positionPrevious = { 0,0 };
}

NPC::~NPC()
{
}


void NPC::setPreviousPosition(NPC * npc, Vector2D pos)
{
	(*npc).positionPrevious.x = pos.x;
	(*npc).positionPrevious.y = pos.y;
}
