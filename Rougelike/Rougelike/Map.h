#pragma once

struct Map
{
	int width = 70;
	int height = 35;

	//char map[70][35] = {};
	char data[70 * 35] = {};

	Map* pointer;

	char playerChar = '@';
	char ground = '.';
};

//void generateBlankMap(Map *map, Player player);
void generateBlankFlatMap(Map *map, Player player);
