#pragma once
class GameManager
{
public:

	enum GameStates
	{
		OVERWORLD,
		BATTLE,
		DEAD
	};

	GameStates gameState = OVERWORLD;
	GameManager();
	~GameManager();
};

