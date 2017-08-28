#include "GameObject.h"

GameObject::GameObject(Vector2D pos, char c, WORD cc)
{
	_position = pos;
	_character = c;
	_characterColor = cc;
}

GameObject::~GameObject()
{
}

