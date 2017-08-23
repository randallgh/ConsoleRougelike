#include "Point2D.h"
#include "Map.h"

int flatArrayIndex(Point2D point, Map map) 
{
	return point.x * point.y * map.width;
}