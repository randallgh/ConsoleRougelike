#include "Player.h"

void setPosition(Player *player, Vector2D pos) {
	(*player).position.x = pos.x;
	(*player).position.y = pos.y;
}

void setPreviousPosition(Player *player, Vector2D pos) {
	(*player).positionPrevious.x = pos.x;
	(*player).positionPrevious.y = pos.y;
}