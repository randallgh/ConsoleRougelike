#pragma once

#include "Point2D.h"
#include "Map.h"


struct Point2D
{
	int x = 0;
	int y = 0;
};

int flatArrayIndex(Point2D point, Map map);
