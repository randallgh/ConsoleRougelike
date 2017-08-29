#include "Vector2D.h"

#include <math.h>

float distanceVector2D(Vector2D a, Vector2D b) {
	float dis = sqrt(pow( (float)b.x - a.x, 2) + pow( (float)b.y - a.y, 2));
	dis = ceil(dis);
	return (int)dis;
}

int vectorToFlatArray(Vector2D vec, int width) {
	return vec.x + width * vec.y;
}