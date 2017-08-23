#include "Player.h"
#include "Map.h"

//void generateBlankMap(Map *map, Player player)
//{
//	//Add ground to map
//
//	for (int y = 0; y < (*map).height; y++)
//	{
//		for (int x = 0; x < (*map).width; x++)
//		{
//			(*map).map[x][y] = (*map).ground;
//		}
//	}
//
//	//Add player to pos
//	(*map).map[player.position.x][player.position.y] = player.character;
//}

void generateBlankFlatMap(Map *map, Player player)
{
	//Add ground to map

	for (int i = 0; i < (*map).height * (*map).width; i++)
	{
		(*map).data[i] = (*map).ground;
	}

	//Add player to pos
	(*map).data[player.position.x * player.position.y] = player.character;
}