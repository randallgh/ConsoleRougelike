#pragma once
class GameManager
{
public:

	enum GameStates
	{
		OVERWORLD,
		BATTLE
	};

	GameStates gameState = OVERWORLD;
	GameManager();
	~GameManager();
};

