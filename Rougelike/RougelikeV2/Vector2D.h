#pragma once

struct Vector2D
{
	int x;
	int y;
};


float distanceVector2D(Vector2D a, Vector2D b);
int vectorToFlatArray(Vector2D vec, int width);