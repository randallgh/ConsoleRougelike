#include "InfoBox.h"



InfoBox::InfoBox(Vector2D start)
{
	for (int i = 0; i < BOXHEIGHT; ++i)
	{
		points[i] = start;
		points[i].y += i;
	}
}

void InfoBox::Clear(char ui[], int uiWidth)
{
	for (Vector2D element : points)
	{
		for (int i = 0; i < BOXWIDTH; ++i)
		{
			ui[element.x + uiWidth * element.y + i] = ' ';
		}
	}
}

void InfoBox::AddInfo(char ui[], int uiWidth, int point, std::string info)
{
	//Clear(ui, uiWidth);
	info.resize(BOXWIDTH);
	for (int i = 0; i < BOXWIDTH; ++i) 
	{
		ui[points[point].x + uiWidth * points[point].y + i] = info.at(i);
	}
}

InfoBox::~InfoBox()
{
}
