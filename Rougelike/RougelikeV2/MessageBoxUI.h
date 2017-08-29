#pragma once
#include "Message.h"
#include "Vector2D.h"

class MessageBoxUI
{
public:
	Message messages[9];
	Vector2D points[9];

	MessageBoxUI();
	MessageBoxUI(Vector2D start);

	void Clear(char ui[], int uiWidth, int messageLength);
	//void Print();
	void Add(Message mes);

	~MessageBoxUI();
};

