#pragma once
class Input
{
public:
	//Get Keydown return true once if the key is pressed down
	//Get Keyup return true once if the key is let go
	//Get Key return true if the key is pressed, returns false if the kay is not pressed
	Input();
	~Input();

	void GetKeyDown(int keyCode);
};

