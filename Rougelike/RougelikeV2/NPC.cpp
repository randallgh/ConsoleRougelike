#include "NPC.h"



NPC::NPC()
{
	this->name = "null";
	this->isAlive = false;
}

NPC::NPC(NPC * npc)
{
	this->character			= npc->character;
	this->characterColor	= npc->characterColor;

	this->health			= npc->health;
	this->maxHealth			= npc->health;

	this->attack			= npc->attack;
	this->level				= npc->level;
	this->dexterity			= npc->dexterity;

	this->exp				= npc->exp;

	this->isAlive			= npc->isAlive;

	this->position.x		= npc->position.x;
	this->position.y		= npc->position.y;
	this->positionPrevious	= npc->positionPrevious;

	this->name				= npc->name;
}


NPC::NPC(char c, WORD cc, std::string name, int heal, int atck, int dexterity, int level)
{
	this->character = c;
	this->characterColor = cc;
	this->name = name;

	this->health = heal;
	this->maxHealth = heal;

	this->dexterity = dexterity;

	this->attack = atck;
	this->level = level;

	this->isAlive = true;

	this->position = { 0,0 };
	this->positionPrevious = { 0,0 };
}

NPC::~NPC()
{

}


void NPC::setPreviousPosition(Vector2D pos)
{
	this->positionPrevious.x = pos.x;
	this->positionPrevious.y = pos.y;
}

void NPC::setPosition(Vector2D pos)
{
	this->position.x = pos.x;
	this->position.y = pos.y;
}
