#include "Vector2D.h"

#include <math.h>

float distanceVector2D(Vector2D a, Vector2D b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

int vectorToFlatArray(Vector2D vec, int width) {
	return vec.x + width * vec.y;
}