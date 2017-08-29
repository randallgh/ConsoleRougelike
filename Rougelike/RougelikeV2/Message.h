#pragma once

class Message
{
public:
	Message();
	Message(char a[], int length);

	char data[50];
	int length = 50;

	~Message();
	

private:
	
};

