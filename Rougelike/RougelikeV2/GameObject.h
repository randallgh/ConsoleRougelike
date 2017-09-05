#pragma once
#include "Vector2D.h"
#include <Windows.h>::WORD

struct GameObject
{
	Vector2D position;
	char character;
	WORD color;
};