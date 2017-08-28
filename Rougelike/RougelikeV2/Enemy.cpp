#include "Enemy.h"

Enemy copyEnemy(Enemy origin) {
	Enemy temp = { origin.health, origin.attack, origin.character};
	return temp;
}

void setPosition(Enemy *enemy, Vector2D pos) {
	(*enemy).position.x = pos.x;
	(*enemy).position.y = pos.y;
}

void setPreviousPosition(Enemy *enemy, Vector2D pos) {
	(*enemy).positionPrevious.x = pos.x;
	(*enemy).positionPrevious.y = pos.y;
}