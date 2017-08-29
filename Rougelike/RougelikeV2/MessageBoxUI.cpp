#include "MessageBoxUI.h"



MessageBoxUI::MessageBoxUI()
{
}

MessageBoxUI::MessageBoxUI(Vector2D start)
{
	for (int i = 0; i < 9; ++i) 
	{
		points[i] = start;
		points[i].y += i;
	}
}

void MessageBoxUI::Clear(char ui[], int uiWidth, int messageLength)
{
	for (Vector2D element : points) 
	{
		for (int i = 0; i < messageLength; ++i) 
		{
			ui[element.x + uiWidth * element.y + i] = ' ';
		}
	}
}

void MessageBoxUI::Add(Message mes)
{
	for (int i = 0; i < 8; ++i) 
	{
		messages[i] = messages[i + 1];
	}

	messages[0] = mes;
}


MessageBoxUI::~MessageBoxUI()
{
}
