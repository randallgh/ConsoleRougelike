#pragma once
#include <string>::std::string

#include "Vector2D.h"::Vector2D


class InfoBox
{
public:

	InfoBox::InfoBox(Vector2D start);

	#define BOXHEIGHT 9
	#define BOXWIDTH 28


	Vector2D points[BOXHEIGHT];

	void Clear(char ui[], int uiWidth);

	//Add a string to a specific point
	void AddInfo(char ui[], int uiWidth, int point, std::string info);

	~InfoBox();
};

