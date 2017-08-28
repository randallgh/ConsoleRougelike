#pragma once
#include "Vector2D.h"
#include <Windows.h>::WORD

/*Used for any object in the game world*/
class GameObject
{
public:
	GameObject(Vector2D pos, char c, WORD cc);
	~GameObject();

private:

protected:
	Vector2D _position;

	char _character;
	WORD _characterColor;
	bool _collison;

	GameObject *pointer;
};

