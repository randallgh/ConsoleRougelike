#pragma once

#include <string>

class Message
{
public:
	Message();
	//Message(char a[], int length);
	Message(std::string str);

	char data[50];
	int length = 50;

	~Message();
	

private:
	
};

