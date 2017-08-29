#include "MessageBoxUI.h"



MessageBoxUI::MessageBoxUI()
{
}

MessageBoxUI::MessageBoxUI(Vector2D start)
{
	for (int i = 0; i < NUMMESSAGES; ++i) 
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

void MessageBoxUI::Print(char ui[], int uiWidth, int messageLength)
{
	for (int i = 0; i < NUMMESSAGES; ++i) 
	{
		for (int c = 0; c < messageLength; ++c) 
		{
			if (messages[i].data[c] == '\0') 
			{
				break;
			}

			ui[points[i].x + uiWidth * points[i].y + c] = messages[i].data[c];
		}
	}
}

void MessageBoxUI::Add(Message mes)
{
	for (int i = NUMMESSAGES - 2; i >= 0; --i) 
	{
		messages[i + 1] = messages[i];
	}

	messages[0] = mes;
}


MessageBoxUI::~MessageBoxUI()
{
}
