#pragma once
#include "Message.h"
#include "Vector2D.h"

class MessageBoxUI
{
public:
	#define NUMMESSAGES 9

	Message messages[NUMMESSAGES];
	Vector2D points[NUMMESSAGES];

	MessageBoxUI();
	MessageBoxUI(Vector2D start);

	void Clear(char ui[], int uiWidth, int messageLength);
	void Print(char ui[], int uiWidth, int messageLength);
	void Add(Message mes);

	~MessageBoxUI();
};

