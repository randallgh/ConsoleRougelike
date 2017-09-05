#pragma once

#include <string>
#include <Windows.h>::WORD

class Message
{
public:
	Message();
	//Message(char a[], int length);
	Message(std::string str);

	char data[50];
	int length = 50;
	WORD color;

	~Message();
	

private:
	
};

