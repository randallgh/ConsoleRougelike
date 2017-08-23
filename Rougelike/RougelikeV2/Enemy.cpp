#include "Enemy.h"

Enemy copyEnemy(Enemy origin) {
	Enemy temp = { origin.health, origin.attack, origin.character};
	return temp;
}