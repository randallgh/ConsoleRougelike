#pragma once

struct Map
{
	int width = 64;
	int height = 20;

	char map[64][20] = {};

	Map* pointer;
};