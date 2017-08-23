#pragma once

#include "Point2D.h"
#include "Map.h"


struct Point2D
{
	int x;
	int y;
};

int flatArrayIndex(Point2D point, Map map);
